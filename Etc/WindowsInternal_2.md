# 8. 시스템 메커니즘 (System Mechanisms)

---
 
## 8-1 프로세서 실행 모델 (Processor Execution Model)
 
### 세그멘테이션 (Segmentation)
 
x86/x64 CPU는 두 가지 메모리 보호 모델을 지원
 
**세그멘테이션**
- 메모리를 세그먼트로 나누는 방식
- x64에서는 사실상 flat 모델로 세그멘테이션을 사용하지 않음
- 구조체 유지
 
세그먼트 레지스터와 역할 :
 
| 레지스터 | 역할 |
|---------|------|
| CS | 코드 세그먼트 (현재 실행 권한 레벨 포함) |
| SS | 스택 세그먼트 |
| DS/ES/FS/GS | 데이터 세그먼트. FS는 유저 모드 TEB, GS는 커널 모드 KPCR 주소 저장 |
 
**GDT (Global Descriptor Table)**
- 시스템 전체 세그먼트 서술자 배열
- CPU당 하나씩 존재
- GDTR 레지스터가 GDT 기준 주소 보관
 
**LDT (Local Descriptor Table)**
- 프로세스별 세그먼트 서술자
- Windows는 실질적으로 사용 안 함
 
### TSS (Task State Segment)
 
CPU별로 하나씩 존재하는 데이터 구조, 하기 두 가지 목적으로 사용
 
- **인터럽트·예외 발생 시 커널 스택 포인터 제공** : 유저 모드에서 커널 모드로 전환 시 새 RSP(스택 포인터) 값을 TSS의 RSP0 필드에서 가져옴
- **IST (Interrupt Stack Table)** : NMI, 더블 폴트, 머신 체크 등 치명적 예외에 전용 스택 제공
x64에서 TSS는 GDT에 등록된 세그먼트로, TR(Task Register) 레지스터가 TSS 위치를 가리킨다
 
---
 
## 8-2 하드웨어 사이드채널 취약점 (Hardware Side-Channel Vulnerabilities)
 
현대 CPU의 성능 최적화 기술(비순서 실행, 분기 예측)이 보안 취약점을 만든다는 사실이 2017~2018년에 공개
 
### 비순서 실행 (Out-of-Order Execution)
 
CPU는 명령어를 프로그램 순서가 아닌 실행 가능한 순서로 먼저 실행한 뒤, 아키텍처 상태는 정상 순서로 커밋

투기 실행(speculative execution) 중 발생한 메모리 접근이 CPU 캐시에 흔적을 남김
 
### CPU 분기 예측기 (Branch Predictor)
 
CPU는 분기 명령의 목적지를 미리 예측해 파이프라인을 채움

예측 테이블은 물리 코어를 공유하는 스레드 간에 공유될 수 있음
 
### CPU 캐시와 사이드채널 공격
 
**Meltdown (CVE-2017-5754)** : 투기 실행 중 커널 메모리를 유저 모드에서 읽을 수 있는 취약점, 인텔 CPU에서 주로 발생
 
**Spectre (CVE-2017-5753, CVE-2017-5715)** : 분기 예측기를 오염시켜 임의 코드 경로를 투기 실행시키는 취약점, 모든 아키텍처에 영향
 
---
 
## 8-3 사이드채널 완화 기법 (Side-Channel Mitigations in Windows)
 
### KVA Shadow (Kernel Virtual Address Shadow)
 
Meltdown 완화를 위해 Windows에서 구현한 기법
 
- 유저 모드 실행 시 : 커널 주소 공간에서 필수 커널 코드(인터럽트 핸들러, 시스템 콜 스텁)만 매핑한 **섀도우 페이지 테이블** 사용
- 커널 모드 전환 시 : 전체 커널 주소 공간이 있는 **정상 페이지 테이블** 로 교체
- 전환 비용을 줄이기 위해 PCID(Process Context ID) 하드웨어 기능 활용
### 하드웨어 간접 분기 제어
 
| 기법 | 설명 |
|------|------|
| IBRS (Indirect Branch Restricted Speculation) | 커널 모드에서 유저 모드의 분기 예측 테이블 영향 차단 |
| IBPB (Indirect Branch Predictor Barrier) | 컨텍스트 전환 시 분기 예측 테이블 초기화 |
| STIBP (Single Thread Indirect Branch Predictors) | SMT에서 같은 물리 코어의 형제 논리 프로세서 간 분기 예측 격리 |
| SSBD (Speculative Store Bypass Disable) | 이전 스토어의 값을 우회하는 투기 로드 차단 |
 
### Retpoline
 
소프트웨어 기반 Spectre v2 완화. 간접 분기를 무한 루프 구조로 교체해 CPU 분기 예측기가 공격자 제어 주소를 투기 실행하지 못하게 한다
 
Windows 커널과 드라이버에 Retpoline이 컴파일러 수준에서 적용됨
 
### STIBP 페어링
 
두 논리 프로세서가 같은 물리 코어에서 실행 중일 때 STIBP를 적용. Windows 스케줄러가 SMT 쌍을 인식해 분기 예측 격리가 필요한 상황을 판단한다
 
---
 
## 8-4 트랩 디스패칭 (Trap Dispatching)
 
인터럽트와 예외는 모두 **트랩**으로 처리된다. CPU가 트랩을 받으면 현재 실행 상태를 저장하고 IDT(Interrupt Descriptor Table)에서 핸들러 주소를 찾아 실행한다
 
```
트랩 발생
    → CPU가 IDT에서 핸들러 주소 조회
    → 스택에 컨텍스트 저장 (KTRAP_FRAME)
    → 핸들러(KiTrapXX) 실행
    → 커널 트랩 디스패처로 제어 이동
    → 처리 완료 후 IRET로 복귀
```
 
### 인터럽트 디스패칭 (Interrupt Dispatching)
 
인터럽트 소스별 처리 경로 :
 
| 소스 | 처리 방법 |
|------|---------|
| 하드웨어 디바이스 | APIC가 CPU에 벡터 신호 → IDT 핸들러 → ISR 호출 |
| 소프트웨어 인터럽트 (APC, DPC) | 소프트웨어가 IRQL 레벨에서 직접 발생 |
| 타이머 인터럽트 | 클록 인터럽트 → 스케줄러·타임아웃 처리 |
 
**IDT (Interrupt Descriptor Table)** : CPU당 하나씩 존재. 256개 항목 (0~255벡터). IDTR 레지스터가 기준 주소 보관
 
**APIC (Advanced Programmable Interrupt Controller)** : 하드웨어 인터럽트를 CPU 코어에 분배. 로컬 APIC(CPU당 1개) + I/O APIC(시스템당 1~수개) 구성
 
### 라인 기반 vs 메시지 신호 기반 인터럽트
 
| 유형 | 설명 |
|------|------|
| 라인 기반 (INTx) | 물리 인터럽트 핀 사용. 공유 가능하지만 충돌 위험 |
| MSI (Message Signaled Interrupt) | 메모리 쓰기로 인터럽트 전달. PCI Express 기본 방식. 공유 불필요, 성능 우수 |
| MSI-X | MSI 확장판. 장치당 최대 2048개 독립 인터럽트 벡터 |
 
### 타이머 처리 (Timer Processing)
 
Windows는 두 종류의 타이머 소스를 사용한다 :
 
- **HPET (High Precision Event Timer)** : 고해상도 타이머. 100ns 단위
- **TSC (Time Stamp Counter)** : CPU 클록 사이클 카운터. 가장 빠른 타임스탬프 소스
타이머 해상도 : 기본 15.6ms (64Hz). `timeBeginPeriod(1)`로 1ms까지 낮출 수 있지만 배터리 소모 증가
 
타이머 합산 (Timer Coalescing) : 비슷한 시간에 만료되는 타이머를 묶어 CPU 깨어남 횟수를 줄여 전력 절감
 
### 시스템 워커 스레드 (System Worker Threads)
 
System 프로세스에 소속된 커널 모드 스레드 풀. 드라이버가 DISPATCH_LEVEL 미만(PASSIVE_LEVEL)에서 처리해야 하는 지연 작업을 위임할 때 사용
 
- `ExQueueWorkItem` API로 워크 아이템 삽입
- 세 종류 : CriticalWorkQueue, DelayedWorkQueue, HyperCriticalWorkQueue
- IRQL = PASSIVE_LEVEL에서 실행 → 페이지 폴트, 대기 가능
### 예외 디스패칭 (Exception Dispatching)
 
CPU가 예외 발생 → 커널 예외 디스패처로 제어 이동 → 처리 순서 :
 
1. 커널 디버거가 연결되어 있으면 첫 번째 통보
2. SEH(구조적 예외 처리) 프레임 탐색 → 처리기 실행
3. 처리되지 않으면 VEH(벡터 예외 처리) 호출
4. 커널 디버거에 두 번째 통보
5. 여전히 미처리 시 UnhandledExceptionFilter → 오류 보고 → 프로세스 종료
### 시스템 서비스 처리 (System Service Handling)
 
유저 모드에서 커널 서비스를 호출하는 경로 :
 
```
유저 모드 앱 → Windows API(Kernel32.dll 등)
    → Ntdll.dll 스텁 함수 (NtXxx / ZwXxx)
    → SYSCALL 명령 (x64) 또는 SYSENTER (x86)
    → KiSystemCall64 (커널 트랩 핸들러)
    → SSDT에서 서비스 번호로 함수 포인터 조회
    → 실제 커널 함수 실행 (NtReadFile 등)
    → SYSRET로 유저 모드 복귀
```
 
SSDT (System Service Descriptor Table) : 시스템 콜 번호 → 커널 함수 포인터 매핑 테이블. PatchGuard 보호 대상
 
---
 
## 8-5 WoW64 (Windows-on-Windows)
 
64비트 Windows에서 32비트 앱을 실행하는 호환성 레이어
 
### WoW64 핵심 구조
 
- `Wow64.dll` : 32비트 → 64비트 시스템 콜 변환 핵심
- `Wow64Win.dll` : Win32k.sys의 32비트 래퍼
- `Wow64Cpu.dll` (x64) / `xtajit.dll` (ARM64) : CPU 상태 전환 담당
32비트 프로세스에 두 개의 Ntdll이 로드된다 :
- `SysWOW64\ntdll.dll` (32비트) : 앱이 직접 호출
- `System32\ntdll.dll` (64비트) : 실제 시스템 콜 수행
### 파일 시스템 리다이렉션
 
32비트 앱이 `%SystemRoot%\System32`에 접근 시 자동으로 `%SystemRoot%\SysWOW64`로 리다이렉트. `SysNative` 경로를 사용하면 실제 System32에 접근 가능
 
### 레지스트리 리다이렉션
 
32비트 앱이 `HKLM\Software`에 접근 시 자동으로 `HKLM\Software\WOW6432Node`로 리다이렉트
 
### ARM64의 이기종 WoW64
 
| 실행 모드 | 구현 |
|---------|------|
| ARM32 on ARM64 | 네이티브 ARM32 실행 |
| x86 on ARM64 | x86 명령어를 동적으로 ARM64 코드로 변환 (CHPE 하이브리드 PE 활용) |
 
---
 
## 8-6 오브젝트 관리자 (Object Manager)
 
Executive 전체에서 단일화된 방식으로 커널 리소스를 생성·관리·삭제하는 커널 컴포넌트
 
### 설계 목표
 
- 단일 인터페이스로 모든 유형의 시스템 리소스 관리
- 리소스에 대한 보안 접근 제어 (7장 참조)
- 참조 카운팅 기반 생명주기 관리
- 오브젝트 핸들 테이블 → 프로세스별 독립 네임스페이스
### Executive 오브젝트 유형
 
| 유형 | 설명 |
|------|------|
| 프로세스 (Process) | 실행 중인 프로세스 |
| 스레드 (Thread) | 실행 스레드 |
| 섹션 (Section) | 공유 메모리·파일 매핑 |
| 파일 (File) | 파일·디바이스·파이프·메일슬롯 |
| 토큰 (Token) | 보안 컨텍스트 |
| 이벤트 (Event) | 동기화 이벤트 |
| 세마포어 (Semaphore) | 카운팅 동기화 |
| 뮤텍스 (Mutex) | 상호 배제 |
| 타이머 (Timer) | 타임아웃 오브젝트 |
| I/O 완료 포트 | 비동기 I/O 완료 통지 |
| 잡 (Job) | 프로세스 그룹 |
| 심볼릭 링크 | 오브젝트 네임스페이스 별칭 |
| 디렉터리 | 오브젝트 네임스페이스 컨테이너 |
 
### 오브젝트 구조
 
모든 오브젝트는 공통 헤더(OBJECT_HEADER) + 유형별 본체로 구성된다 :
 
```
OBJECT_HEADER
├── PointerCount    : 총 참조 카운트 (커널 포인터 포함)
├── HandleCount     : 핸들 참조 카운트
├── TypeIndex       : 오브젝트 유형 포인터 → OBJECT_TYPE
├── SecurityDescriptor : 보안 서술자
└── [선택적 헤더들] : 이름 정보, 프로세스 할당량, 핸들 DB 등
 
OBJECT_TYPE (유형 오브젝트)
├── TypeInfo       : 각종 한도 및 플래그
└── 메서드 포인터들 : Open, Close, Delete, Parse, Security, QueryName 등
```
 
참조 카운팅 규칙 :
- 핸들 오픈 → HandleCount + PointerCount 증가
- 핸들 닫기 → HandleCount 감소
- PointerCount = 0이면 Delete 메서드 호출 후 해제
### 오브젝트 네임스페이스
 
오브젝트 관리자가 관리하는 계층적 이름 공간 :
 
```
\
├── Device\         : 디바이스 오브젝트
├── Driver\         : 드라이버 오브젝트
├── GLOBAL??\       : DOS 드라이브 심볼릭 링크 (C:, D: 등)
├── KernelObjects\  : 전역 이벤트 오브젝트
├── Sessions\       : 세션별 오브젝트
│   └── N\
│       ├── BaseNamedObjects\    : 유저 모드 명명 오브젝트
│       └── AppContainerNamedObjects\ : AppContainer 전용
├── Registry\       : 레지스트리 루트
└── Windows\        : 윈도우 스테이션
```
 
`WinObj` (Sysinternals) 도구로 전체 네임스페이스 탐색 가능
 
### 핸들 테이블
 
모든 프로세스는 독립적인 핸들 테이블을 가진다 :
 
- 3단계 트리 구조 (최대 16M개 핸들)
- 각 항목 = 오브젝트 포인터 + 접근 마스크 + 상속·보호 플래그
- 핸들 번호는 4의 배수 (비트 0~1은 플래그)
전역 핸들 (−1, −2, −3) : 현재 프로세스·스레드·프로세스 토큰을 나타내는 의사(pseudo) 핸들
 
---
 
## 8-7 동기화 (Synchronization)
 
커널 코드가 공유 데이터 구조를 보호하기 위한 메커니즘
 
### 높은 IRQL 동기화
 
**스핀락 (Spin Lock)** : IRQL = DISPATCH_LEVEL(2) 이상에서 사용. 잠금을 획득할 때까지 CPU를 바쁜 대기(busy-wait)로 소비. 단일 프로세서에서는 스핀락 대신 IRQL만 올림
 
- `KeAcquireSpinLock` / `KeReleaseSpinLock`
- 잠금 보유 중에는 페이지 폴트 발생 불가 → 논페이지드 메모리 필수
- 잠금 보유 시간을 최소화해야 함 (다른 CPU가 스핀 대기 중)
**큐드 스핀락 (Queued Spin Lock)** : FIFO 순서로 대기자를 줄 세워 공정성 확보. `KeAcquireInStackQueuedSpinLock`
 
**인터락 연산** : 스핀락 없이 원자적으로 단일 값을 읽고 쓰는 CPU 명령 활용. `InterlockedIncrement`, `InterlockedCompareExchange` 등
 
### 낮은 IRQL 동기화
 
IRQL 0~1에서 사용. 잠금 실패 시 스레드를 대기 상태로 전환 (블로킹)
 
| 오브젝트 | 설명 |
|---------|------|
| 뮤텍스 (Mutex/KMUTEX) | 소유자 개념 있는 상호 배제. 소유자가 재진입 가능. 스레드 종료 시 자동 해제(abandoned) |
| 이벤트 (Event/KEVENT) | 알림(Notification) : 수동 리셋, 신호 시 모든 대기자 해제. 동기화(Synchronization) : 자동 리셋, 신호 시 대기자 하나만 해제 |
| 세마포어 (Semaphore) | 최대 N개 스레드 동시 진입 허용 카운팅 세마포어 |
| 타이머 (Timer) | 절대·상대 시각 기반 타임아웃. DPC와 결합 가능 |
| 게이트 (Gate) | 이벤트 경량 버전. 유저 모드 노출 없음 |
 
**디스패처 오브젝트 대기** : `KeWaitForSingleObject` / `KeWaitForMultipleObjects`. 내부적으로 KWAIT_BLOCK 구조체를 오브젝트의 대기 리스트에 삽입하고 스레드를 Waiting 상태로 전환
 
**푸시락 (Push Lock)** : 읽기·쓰기 구분이 있는 경량 동기화 오브젝트. 경쟁이 없는 경우 단일 원자 연산으로 잠금 획득 가능
 
**EResource** : 다중 읽기·단일 쓰기 지원. 페이지드 코드에서 주로 사용. 파일 시스템 드라이버가 애용
 
---
 
## 8-8 ALPC (Advanced Local Procedure Call)
 
Windows 내부 프로세스 간 통신(IPC)의 핵심 메커니즘. Win32 RPC, COM, Named Pipe, Mailslot이 모두 ALPC 위에 구현됨
 
### 연결 모델
 
```
서버
    NtCreateAlpcPort → \RPC Control\서버이름 등록
    NtAlpcConnectPort → 연결 대기
 
클라이언트
    NtAlpcConnectPort → 서버에 연결 요청
    → 서버가 NtAlpcAcceptConnectPort로 수락
    → 양쪽에 고유한 연결 포트 생성
```
 
### 메시지 모델
 
ALPC 메시지 크기에 따른 전달 방식 :
 
| 메시지 크기 | 전달 방식 |
|-----------|---------|
| 0~256바이트 | 메시지 큐에 직접 복사 |
| 257~65536바이트 | 섹션(공유 메모리) 뷰를 통해 데이터 전달 |
| 65536바이트 이상 | 핸들 전달로 대용량 데이터 처리 |
 
### 주요 특성
 
- **비동기 동작** : 비동기 포트로 비동기 메시지 전송. I/O 완료 포트와 통합 가능
- **보안** : 포트 오브젝트에 보안 서술자 적용. 연결 요청 시 클라이언트 신원 확인 가능
- **Direct Event Attribute** : 연결된 이벤트 핸들로 직접 신호 가능 (메시지 없이)
- **핸들 전달** : NtAlpcSendWaitReceivePort로 오브젝트 핸들을 다른 프로세스에 안전하게 전달
- **전원 관리 통합** : 배터리 절약 정책과 연동해 저전력 대기 상태에서 메시지 처리 제어
---
 
## 8-9 WNF (Windows Notification Facility)
 
시스템 전반의 상태 변경을 발행-구독 모델로 효율적으로 전파하는 알림 메커니즘
 
### 특징
 
- **커널 통합** : 커널이 직접 WNF 상태 변경을 발행 가능 (파일 시스템, 전원 관리 등)
- **영속성** : 프로세스 재시작 후에도 마지막 상태 값이 유지됨 (레지스트리 또는 메모리에 저장)
- **효율성** : 폴링 없이 변경 사항만 전달. 동일 시간에 여러 변경이 발생하면 집계(aggregate)해 한 번에 전달
### WNF 상태 이름과 저장소
 
WNF 상태 이름은 64비트 숫자로 인코딩. 저장소 유형 :
 
| 저장소 | 설명 |
|--------|------|
| 임시 (Temporary) | 프로세스 재시작 시 소멸 |
| 영구 (Permanent) | HKLM\System\CurrentControlSet\Control\Notifications에 저장 |
| 런타임 (Runtime) | 메모리 내 전역 공유 섹션에 저장 |
| 디바이스 상태 (Device State) | 레지스트리에 영구 저장 |
 
### 이벤트 집계 (Event Aggregation)
 
짧은 시간 내 다수의 변경 이벤트를 하나로 병합해 전달. 빈번한 상태 변경(예: 배터리 수준, 네트워크 상태)에서 과도한 콜백 호출 방지
 
---
 
## 8-10 유저 모드 디버깅 (User-Mode Debugging)
 
### 커널 지원
 
커널이 디버깅을 지원하기 위해 제공하는 기반 :
 
- `NtCreateDebugObject` : 디버그 오브젝트 생성
- 디버그 대상 프로세스에 `DbgkCreateThread`, `DbgkMapViewOfSection`, `DbgkExitThread` 등의 이벤트 자동 전송
- 디버거가 `NtDebugActiveProcess`로 실행 중인 프로세스에 붙기(attach) 가능
### 네이티브 디버깅 지원
 
Ntdll.dll의 DbgSs(Dbg Sub-System) 루틴이 시스템 콜과 디버그 이벤트를 중계
 
예외 발생 시 디버거에 두 번의 통보 기회 :
 
1. **First-chance** : 예외 핸들러보다 먼저 디버거에 알림 (디버거가 처리하거나 패스)
2. **Second-chance** : 모든 핸들러가 처리 못한 후 디버거에 재알림
### Windows 서브시스템 디버깅 지원
 
Win32 API의 `DebugActiveProcess` → 내부적으로 `NtDebugActiveProcess` → Csrss.exe에도 알림. 모든 디버거(WinDbg, Visual Studio 등)는 이 경로로 통일
 
---
 
## 8-11 패키지 앱 (Packaged Applications)
 
Windows 8부터 도입된 .Appx 패키지 형식으로 배포되는 앱
 
### UWP 앱 (Universal Windows Platform)
 
- AppContainer 보안 컨텍스트에서 실행 (7장 참조)
- PLM(Process Lifetime Manager)이 앱 실행 상태 제어 (실행·일시 중단·종료)
- State Repository에 앱 상태·설정 저장
### Centennial 앱 (데스크톱 브리지)
 
기존 Win32 앱을 .Appx 패키지로 포장한 앱. AppContainer 없이 실행되지만 패키지 설치·업데이트·제거를 스토어를 통해 처리
 
### 핵심 구성 요소
 
| 구성 요소 | 역할 |
|---------|------|
| HAM (Host Activity Manager) | 앱 활성화·비활성화 생명주기 관리. 백그라운드 작업 조율 |
| State Repository | 패키지 상태 및 설정 영속 저장소. SQLite 기반 |
| Dependency Mini Repository | 패키지 간 의존성 추적 |
| Background Broker Infrastructure | 타이머·네트워크·푸시 알림 등 백그라운드 트리거 처리 |
 
### 패키지 활성화 (Package Activation)
 
1. 사용자 또는 시스템이 앱 실행 요청
2. PLM이 HAM을 통해 앱 활성화 가능 여부 판단
3. State Repository에서 패키지 정보 조회
4. 새 프로세스 생성 또는 기존 프로세스 포그라운드로 전환
5. AppContainer 토큰 생성 및 적용
6. Activation Factory를 통해 앱 진입점 호출
### 패키지 등록 (Package Registration)
 
`Add-AppxPackage` 또는 앱 스토어를 통해 설치 시 :
 
- State Repository에 패키지 메타데이터 등록
- AppContainer SID 생성 및 파일 시스템·레지스트리 ACL 설정
- COM 클래스·프로토콜 핸들러 등록
- 바로가기·타일 등록

# 9. 가상화 기술 (Virtualization Technologies)

---

## 9-1 가상화 개요

현대 CPU가 제공하는 하드웨어 가상화 확장(Intel VT-x, AMD-V, ARM Virtualization Extensions)을 기반으로 단일 물리 머신에서 여러 OS를 동시에 실행하거나, VBS(가상화 기반 보안)처럼 OS 자체를 보호하는 데 활용된다.

Windows 하이퍼바이저의 핵심 설계 원칙은 **최소화(microkernel-like)** 다. 드라이버 지원이나 I/O 처리를 직접 내장하지 않고, 실제 가상화 작업의 대부분을 루트 파티션의 **가상화 스택**에 위임한다.

---

## 9-2 파티션 (Partition)

하이퍼바이저의 핵심 격리 단위. OS 인스턴스 하나가 곧 파티션 하나다. Host / Guest 대신 **루트 파티션 / 차일드 파티션** 으로 구분한다.

```
하이퍼바이저
├── 루트 파티션 (Root Partition)
│   ├── Windows OS (가상화 스택 제공)
│   ├── 가상화 스택 (VMMS, VM Worker Process 등)
│   └── 실제 하드웨어 드라이버
└── 차일드 파티션 (Child Partition, 0개 이상)
    └── 게스트 OS (Windows, Linux 등)
```

### 루트 파티션

- 물리 하드웨어에 대한 직접 접근 권한 보유 (하이퍼바이저·보안 커널 물리 메모리 제외)
- 가상화 스택 전체를 제공·관리
- 모든 차일드 파티션의 관리 포인트
- **Microsoft가 서명한 Windows 만** 루트 파티션이 될 수 있음

### 차일드 파티션

- GPA(Guest Physical Address) 공간만 접근 가능. 물리 하드웨어 직접 접근 불가
- 하이퍼바이저가 모든 I/O를 가로채 루트 파티션으로 리다이렉트
- 파티션 상태를 저장·일시 중지·재개 가능

### 파티션 계층

파티션은 계층적으로 구성된다. 루트가 각 차일드를 제어하고, 차일드에서 발생하는 특정 이벤트를 인터셉트(intercept) 받는다.

---

## 9-3 가상화 스택 (Virtualization Stack)

루트 파티션이 제공하는 가상 머신 관리·통신 컴포넌트 집합.

### 주요 구성 요소

| 컴포넌트 | 역할 |
|---------|------|
| VMMS (vmms.exe) | 가상 머신 관리 서비스. VM 생성·시작·중지 조율 |
| VM Worker Process (vmwp.exe) | VM당 하나씩 생성. VM 상태 관리·에뮬레이션 |
| VID.sys (Virtual Infrastructure Driver) | 하이퍼바이저와 통신하는 커널 모드 드라이버. 파티션·VP 생성, 메모리 관리 |
| WMI Provider | Hyper-V 관리 API 노출 |
| VSP (Virtualization Service Provider) | 루트 파티션 측 가상 디바이스 제공자 |
| VSC (Virtualization Service Client) | 차일드 파티션 측 가상 디바이스 클라이언트 |
| VMBus | VSP와 VSC를 연결하는 고성능 가상 버스 |

### VMBus

루트의 VSP와 차일드의 VSC를 연결하는 빠른 통신 채널. 네트워크·스토리지·그래픽 등 대부분의 I/O가 VMBus를 통해 처리된다. 직접적인 하드웨어 접근보다 오버헤드가 있지만, SR-IOV 같은 디바이스 패스스루(passthrough)로 특정 장치를 차일드가 직접 제어할 수도 있다.

---

## 9-4 하이퍼바이저 내부 구조 (Hypervisor Internals)

### VP (Virtual Processor)

가상 프로세서. 파티션은 하나 이상의 VP로 구성된다. VM_VP 데이터 구조가 VP의 모든 상태를 추적한다.

```
VM_VP
├── 플랫폼 의존 레지스터 상태 (범용, 디버그, XSAVE 영역 등)
├── VP 전용 주소 공간
├── VM_VPLC 배열 → 각 VTL의 상태 추적
├── 백킹 스레드 포인터 (루트 파티션의 실제 Windows 스레드)
└── 현재 실행 중인 물리 프로세서 포인터
```

VP는 루트 파티션의 **Windows 스레드**(백킹 스레드)로 구현된다. 즉, 하이퍼바이저 스케줄러는 결국 어떤 Windows 스레드를 어떤 물리 CPU에서 실행할지를 결정하는 것이다.

### 하이퍼바이저 초기화 순서

1. HvLoader가 CPU 제조사별 하이퍼바이저 바이너리 로드 (Intel = Hvix64.exe, AMD = Hvax64.exe, ARM64 = Hvaa64.exe)
2. `KiSystemStartup` → `BmpInitBootProcessor` 호출 (부트 프로세서만)
3. CPU의 가상화 기능 쿼리 (EPT, VPID 등)
4. 스케줄러 타입 결정 (클라이언트 기본값 = 루트 스케줄러, 서버 기본값 = 코어 스케줄러)
5. 메모리 관리자 초기화 (PFN 데이터베이스 + 루트 컴파트먼트)
6. 하이퍼바이저용 HAL 초기화
7. 프로세스·스레드 서브시스템 초기화
8. VAL (Virtualization Abstraction Layer) 초기화 (Intel EPT, AMD NPT, ARM Stage-2 등 추상화)
9. SynIC, IOMMU 초기화
10. 루트 파티션 생성 + BSP VP 시작

### CPU_PLS

하이퍼바이저에서 물리 프로세서를 나타내는 구조체. NT 커널의 KPRCB에 대응. GS 세그먼트로 빠르게 접근.

---

## 9-5 루트 파티션과 BSP VP 생성

루트 파티션 생성도 차일드 파티션과 같은 규칙을 따른다. 3개 레이어 순서대로 초기화된다.

| 레이어 | 역할 |
|--------|------|
| VM 레이어 | 최대 VTL 수 설정, 파티션 타입에 따른 권한(privilege) 설정 |
| VP 레이어 | 가상화된 CPUID 데이터 초기화, 파티션을 백킹하는 하이퍼바이저 프로세스 생성 |
| AM (Address Manager) 레이어 | 파티션의 초기 물리 주소 공간 구성 (루트 파티션은 Identity Mapping 사용) |

Identity Mapping : 루트 파티션의 GPA = SPA. 즉 게스트 물리 주소 0x1000은 실제 물리 주소 0x1000. 차일드는 GPA ≠ SPA.

---

## 9-6 하이퍼바이저 메모리 관리자

NT 커널이나 보안 커널에 비해 단순한 구조. 핵심 단위는 **메모리 컴파트먼트(Memory Compartment)** 다.

### 컴파트먼트 구조

- NUMA 노드별로 물리 페이지 목록을 보관
- PFN 데이터베이스로 각 물리 페이지 상태와 NUMA 리스트 링크 추적
- 부모-자식 관계 : 자식 컴파트먼트 생성 시 부모(루트)에서 페이지 가져옴. 삭제 시 반환

### 메모리 할당 실패 시 동작

| 요청 주체 | 실패 처리 |
|---------|---------|
| 하이퍼바이저 내부 서비스 (루트 파티션 대행) | 불가 → 시스템 크래시 |
| 차일드 파티션 (하이퍼콜 통해 요청) | INSUFFICIENT_MEMORY 반환 → 루트 파티션이 HvDepositMemory 하이퍼콜로 페이지 보충 |

### 하이퍼바이저 가상 주소 공간

NT 커널이 KVA 섀도잉으로 두 개의 페이지 테이블을 사용하는 것과 달리, 하이퍼바이저는 **단일 루트 페이지 테이블**로 동작. 대신 **Zone** 개념으로 VP별 전용 가상 주소 범위를 제공.

---

## 9-7 파티션의 물리 주소 공간과 SLAT

게스트 OS가 "물리 주소"라고 인식하는 GPA를 실제 물리 주소 SPA로 변환하는 하드웨어 기능이 SLAT다.

| CPU 제조사 | SLAT 명칭 |
|----------|---------|
| Intel | EPT (Extended Page Tables) |
| AMD | NPT (Nested Page Tables) |
| ARM | Stage-2 Address Translation |

x64 페이지 테이블과 유사하게 4단계 변환 구조. 게스트 OS 내부의 가상 주소 변환(CR3 기반)과 GPA→SPA 변환이 **2단계로 중첩**되어 동작한다.

### VTL별 SLAT 테이블

루트 파티션의 각 VTL은 동일한 GPA↔SPA 매핑을 가지지만 보호 수준(protection level)이 다르게 설정된다. 이를 통해 VTL 0과 VTL 1의 메모리 격리가 구현된다. 하이퍼바이저 자체 페이지는 SLAT 테이블에서 invalid 엔트리로 표시되어 어떤 파티션에서도 접근 불가.

---

## 9-8 엔라이트먼트 (Enlightenments)

게스트 OS(주로 Windows 커널)가 자신이 하이퍼바이저 아래에서 실행 중임을 인식하고 동작 방식을 변경하는 최적화 기법.

### 주요 엔라이트먼트 예시

**TLB 플러시 최적화**

일반 멀티프로세서 환경에서 TLB 플러시는 모든 활성 CPU에 IPI(Inter-Processor Interrupt)를 보내야 한다. 가상화 환경에서는 여러 VM의 VP가 같은 물리 CPU를 공유하므로, 불필요한 VM까지 TLB를 플러시하게 된다. 엔라이트된 OS는 TLB 플러시 대신 하이퍼콜을 발행해 해당 파티션에 속한 정보만 선택적으로 플러시한다.

**스핀락 엔라이트먼트**

게스트 커널이 스핀락 활성 대기에 진입할 때 하이퍼콜로 하이퍼바이저에 알림 → 하이퍼바이저가 해당 VP를 선점하고 다른 VP를 스케줄링 → 불필요한 CPU 사이클 낭비 방지.

**APIC 가상화**

인터럽트 상태 진입·탈출과 APIC 접근을 하이퍼바이저와 협력해 처리. 실제 APIC 트래핑 없이 가상화.

### 엔라이트먼트 활성화 조건

게스트 OS가 CPUID를 통해 하이퍼바이저 존재 여부와 기능을 확인한 후 해당 하이퍼바이저 인터페이스를 활성화. Windows 커널(Winhv.sys), Linux(LinuxHv)가 각각 구현.

---

## 9-9 파티션 권한, 속성, 버전 기능

파티션 생성 시 VID 드라이버가 하이퍼바이저에 요청해 권한(Privilege)을 설정한다.

### 파티션 권한

**VP가 시작되기 전**에만 설정 가능. 권한은 파티션이 수행할 수 있는 하이퍼콜·합성 MSR 접근 범위를 정의.

| 파티션 유형 | 주요 권한 예시 |
|-----------|-------------|
| 루트 + 차일드 공통 | VP의 런타임 카운터 R/W, 참조 시각 읽기, SynIC 타이머 접근, 가상 APIC 보조 페이지 R/W |
| 루트 파티션 전용 | 차일드 파티션 생성, 파티션 ID로 참조, 메모리 입출금, 하이퍼바이저 디버거 활성화 |
| 차일드 파티션 전용 | 루트 스케줄러 VP 이벤트 알림, 확장 하이퍼콜 인터셉트 생성 |
| EXO 파티션 | 권한 없음 |

### 파티션 속성

권한과 달리 **언제든지** 설정·조회 가능.

| 속성 그룹 | 설명 |
|---------|------|
| 스케줄링 속성 | VP 시간 슬라이스, NUMA 친화도 설정 |
| 시각 속성 | 게스트 시각 동기화 설정 |
| 일시 중단·재개 허용 | VM 저장·재개 가능 여부 |
| 디버깅 속성 | 하이퍼바이저 디버거 런타임 구성 |
| 리소스 속성 | 메모리 한도 등 리소스 제한 |
| 호환성 속성 | VP에 노출하는 가상 하드웨어 기능 버전 제어 |

---

## 9-10 주소 공간 격리 (HyperClear)

Spectre, Meltdown, Foreshadow 같은 CPU 사이드채널 취약점은 VM 경계를 넘어 다른 VM이나 하이퍼바이저의 메모리를 추론적으로 읽는 데 악용된다.

하이퍼바이저는 **HyperClear** 완화로 이를 방어한다.

### HyperClear 세 가지 구성 요소

| 구성 요소 | 설명 |
|---------|------|
| 코어 스케줄러 | SMT 코어의 두 LP에 항상 같은 VM의 VP만 스케줄링 → 다른 VM이 같은 물리 코어의 캐시를 공유하지 못함 |
| VP 전용 주소 공간 격리 | 하이퍼바이저가 VP별 전용 가상 주소 범위를 유지. 하이퍼바이저 컨텍스트 진입 시 비밀 데이터가 현재 VP 또는 공유 데이터만 주소 지정 가능 |
| 민감 데이터 스크러빙 | 하이퍼바이저가 비밀 데이터에 접근해야 할 때 다른 VP가 형제 SMT 스레드에서 실행되지 않음을 보장 |

### 개인 주소 공간 (Private Address Space)

하이퍼바이저는 단일 루트 페이지 테이블을 사용하지만, 전역 루트 페이지 테이블에 두 개의 PML4 항목(1TB 가상 주소 범위)을 예약한다. VP 생성 시 스택과 전용 데이터 구조를 위한 **개인 페이지 테이블 루트 항목** 두 개가 할당된다. VP 전환 시 이 두 항목만 교체하면 전체 주소 공간이 전환되는 효율적인 방식이다.

---

## 9-11 동적 메모리 (Dynamic Memory)

VM에 할당된 물리 메모리를 런타임에 동적으로 추가·제거하는 기능.

### 동작 원리

PFN 데이터베이스는 최대 가능 물리 주소(64비트 기준 256TB)를 매핑할 수 있도록 VA 공간을 예약해두고, 실제 물리 메모리 범위에 대해서만 대형 페이지로 PFN 항목을 매핑한다. 이 구조 덕분에 메모리 핫애드(hot-add)와 핫리무브(hot-remove)가 가능하다.

구현에 관여하는 3개 컴포넌트 :

| 컴포넌트 | 역할 |
|---------|------|
| NT 커널 메모리 관리자 | 물리 메모리 핫애드·핫리무브 지원 (베어메탈에서도 동작) |
| 하이퍼바이저 AM (Address Manager) | SLAT 테이블을 통해 게스트에게 노출되는 물리 주소 공간 조정 |
| Vmdynmem.dll (VM Worker Process) | VMBus Dynamic Memory VSC 드라이버(Dmvsc.sys)와 통신해 차일드 파티션에 메모리 추가·제거 |

---

## 9-12 Hyper-V 스케줄러

하이퍼바이저는 어떤 VP(백킹 스레드)를 어떤 물리 CPU에서 실행할지 결정하는 독자적인 스케줄러를 가진다. 세 가지 스케줄러 타입이 있다.

### 스케줄러 타입 확인·변경

```powershell
# 현재 스케줄러 타입 확인 (이벤트 로그)
Get-WinEvent -FilterHashTable @{
    ProviderName="Microsoft-Windows-Hyper-V-Hypervisor"; ID=2
} -MaxEvents 1
```

```cmd
# 스케줄러 타입 변경 (재부팅 필요)
bcdedit /set hypervisorschedulertype Core
```

이벤트 ID 2의 스케줄러 코드 : 1 = Classic(SMT 비활성), 2 = Classic, 3 = Core, 4 = Root

### 클래식 스케줄러 (Classic Scheduler)

Hyper-V 초기 버전부터 사용된 기본 스케줄러.

- 라운드 로빈 정책 : 모든 VP에 균등한 시간 슬라이스
- NUMA 토폴로지 인식
- VP 친화도 설정 가능
- 게스트가 현재 무엇을 실행 중인지 모름

세 가지 미세 조정 옵션 :

| 옵션 | 설명 |
|------|------|
| VP 예약 (Reservation) | 물리 CPU 용량의 최소 보장 비율 설정. 해당 용량이 확보될 때만 VP 스케줄 |
| VP 한계 (Limit) | 물리 CPU 사용률 최대 비율 제한. 높은 부하 시 시간 슬라이스 감소 |
| VP 가중치 (Weight) | 예약 충족 후 VP 스케줄 확률 조정. 기본값은 모두 동일 확률 |

**단점** : 고부하 초과 구독 환경에서 성능 저하. SMT 코어의 두 LP에 서로 다른 VM의 VP가 스케줄될 수 있어 사이드채널 공격에 취약.

### 코어 스케줄러 (Core Scheduler)

Windows Server 2016에서 도입. HyperClear의 핵심 구성 요소.

- SMT 코어 단위로 VP를 스케줄 : 코어의 두 LP에는 항상 같은 VM의 VP
- 다른 VM의 VP가 같은 물리 코어의 캐시를 공유하지 못함 → 사이드채널 격리
- VM이 게스트 내부에서 SMT 동작과 API를 정상적으로 사용 가능
- Windows Server 기본값 : 코어 스케줄러

**단점** : 물리 코어 수보다 많은 VM VP 조합이 불가. VP가 idle인 경우 형제 LP도 다른 VM에 줄 수 없어 CPU 낭비 발생 가능.

### 루트 스케줄러 (Root Scheduler)

Windows 10 클라이언트 기본값.

- 루트 파티션의 NT 커널 스케줄러가 VP 백킹 스레드의 스케줄링을 전적으로 담당
- 하이퍼바이저는 스케줄링 결정을 루트 파티션에 위임
- WSL 2와 같은 경량 VM 시나리오에 최적화

---

## 9-13 보안 커널과 VSM (VBS)

파티션 모델은 Windows 자체의 보안 강화에도 활용된다. 루트 파티션 내부에서 VTL 1의 보안 커널(Secure Kernel)을 별도 격리 환경으로 운영하는 것이 VSM이다.

### VTL과 SLAT 보호

각 VTL은 독립된 SLAT 보호 수준을 가진다. VTL 0(일반 커널)은 VTL 1 메모리에 접근하는 SLAT 항목이 invalid로 표시되어 있어 물리적으로 접근 불가.

### 보안 커널 초기화 (VSM Startup)

1. NT 커널이 하이퍼바이저에 VTL 1 활성화 요청 (Access VSM 권한 필요)
2. 하이퍼바이저가 VTL 1 전용 메모리 배치
3. 보안 커널(Securekernel.exe)이 VTL 1에서 실행 시작
4. Trustlet(LsaIso.exe, BioIso.exe 등) 로드

### VSM 시스템 콜

VTL 0 ↔ VTL 1 전환은 **VMCALL** 명령을 사용. 보안 커널은 IUM 전용 시스템 콜만 허용하며, 일반 NT 시스템 콜은 VTL 1에서 실행 불가.

### UEFI 런타임 가상화

UEFI 런타임 서비스(SetVariable 등)는 원래 물리 주소를 직접 조작하므로 SLAT 보호를 우회할 수 있다. 하이퍼바이저는 UEFI 런타임 서비스 호출을 가로채어 별도의 가상화된 환경에서 안전하게 실행한다.

---

## 9-14 VBS 기반 메모리 엔클레이브

VSM을 기반으로 앱 주소 공간 내에 OS와 하이퍼바이저도 접근할 수 없는 격리된 메모리 영역을 제공하는 기능 (Windows 10 1709~).

SGX(Intel Software Guard Extensions)과 달리 VBS 엔클레이브는 CPU 하드웨어 의존 없이 하이퍼바이저 SLAT 보호로 구현된다.

### VBS 엔클레이브 생성 흐름

```
유저 모드 앱
    CreateEnclave(ENCLAVE_TYPE_VBS) → 엔클레이브 VaD 생성
    LoadEnclaveData                  → 엔클레이브 메모리에 코드/데이터 복사
    InitializeEnclave                → 엔클레이브 확정·검증
    [엔클레이브 진입]                → VTL 1의 보안 커널이 관리
    [엔클레이브 탈출]                → 일반 VTL 0 컨텍스트 복귀
```

엔클레이브 내부 코드는 VTL 0의 커널도 읽기 불가. SLAT를 통해 VTL 1만 해당 물리 페이지에 접근 가능하도록 보호된다.

---

## 9-15 중첩 가상화 (Nested Virtualization)

하이퍼바이저 안에서 또 다른 하이퍼바이저를 실행하는 기능.

- L0 하이퍼바이저 : 실제 하드웨어를 관리하는 루트 하이퍼바이저
- L1 하이퍼바이저 : 가상 머신 안에서 실행되는 게스트 하이퍼바이저

엔라이트먼트의 연장선으로 **중첩 엔라이트먼트(Nested Enlightenments)** 가 도입됐다. L1 하이퍼바이저가 자신이 가상화 환경 안에서 실행 중임을 인식하고 하이퍼콜을 통해 L0에 협력 요청을 보낼 수 있다.

VPID(Virtual Processor ID) : TLB 엔트리에 태그를 달아 VM 전환 시 전체 TLB 플러시 없이 파티션별로 무효화하는 하드웨어 기능. 중첩 가상화 환경에서 성능 저하를 줄이는 데 활용.

# 10. 관리, 진단, 추적 (Management, Diagnostics, and Tracing)

---

## 10-1 레지스트리 (Registry)

### 개요

레지스트리는 시스템 구성, 앱 설정, 사용자 프로필, 하드웨어 정보를 저장하는 Windows의 중앙 데이터베이스다. Executive의 구성 관리자(Configuration Manager, `Cm` 접두사)가 담당한다.

### 레지스트리 데이터 타입

| 타입 | 설명 |
|------|------|
| REG_SZ | 유니코드 문자열 (null 종료) |
| REG_EXPAND_SZ | 환경 변수 포함 문자열 (%SystemRoot% 등) |
| REG_MULTI_SZ | null 구분 다중 문자열 배열 |
| REG_DWORD | 32비트 정수 |
| REG_QWORD | 64비트 정수 |
| REG_BINARY | 이진 데이터 |
| REG_LINK | 다른 키를 가리키는 심볼릭 링크 |

### 레지스트리 논리 구조 (5개 루트 키)

| 루트 키 | 약칭 | 내용 |
|--------|------|------|
| HKEY_LOCAL_MACHINE | HKLM | 시스템 전체 하드웨어·소프트웨어·보안 설정 |
| HKEY_CURRENT_USER | HKCU | 현재 로그온 사용자 설정. HKU의 심볼릭 링크 |
| HKEY_CLASSES_ROOT | HKCR | 파일 연결, COM 클래스. HKLM\Software\Classes + HKCU\Software\Classes 병합 뷰 |
| HKEY_USERS | HKU | 모든 사용자 프로필 (SID별 하위 키) |
| HKEY_CURRENT_CONFIG | HKCC | 현재 하드웨어 프로필. HKLM\SYSTEM\CurrentControlSet\Hardware Profiles\Current 링크 |

### HKLM 주요 하위 키

| 하위 키 | 내용 |
|--------|------|
| HKLM\SAM | 로컬 계정·그룹 정보 |
| HKLM\SECURITY | 보안 정책, 도메인 신뢰, 캐시된 자격증명 |
| HKLM\SOFTWARE | 앱·컴포넌트 설정 (32비트는 WOW6432Node 하위 리다이렉트) |
| HKLM\SYSTEM | 부팅 및 드라이버 설정. CurrentControlSet 포함 |
| HKLM\HARDWARE | 동적 생성 하드웨어 정보 (휘발성) |
| HKLM\BCD00000000 | 부트 설정 데이터(BCD). 부트 관리자 옵션 |

### 하이브 파일 위치

`%SystemRoot%\System32\config` 폴더에 SYSTEM, SOFTWARE, SAM, SECURITY, DEFAULT 파일로 저장. 사용자 프로필은 `%UserProfile%\NTUSER.DAT`.

### 레지스트리 내부 구조 (셀 기반)

하이브는 **셀(Cell)** 을 기본 단위로 하는 블록 기반 데이터베이스다.

| 셀 타입 | 내용 |
|--------|------|
| CM_KEY_NODE (Key Cell) | 키 이름, 수정 시각, 하위 키·값 목록 포인터 |
| CM_KEY_VALUE (Value Cell) | 값 이름, 타입, 데이터 |
| CM_KEY_SECURITY (Security Cell) | 보안 서술자 (여러 키가 공유 가능) |
| Index Cell | 하위 키를 정렬된 트리로 관리 |

**하이브 구조**: 기본 블록(매직 넘버 "regf", 시퀀스 번호, 루트 셀 오프셋) + 4KB 단위 빈(Bin)으로 구성

### 레지스트리 네임스페이스

오브젝트 관리자 네임스페이스 `\REGISTRY` 아래 마운트됨.
- `\REGISTRY\MACHINE` → HKLM
- `\REGISTRY\USER` → HKU

### 안정적 저장 (Stable Storage)

각 하이브는 **로그 파일** (.LOG1, .LOG2)로 쓰기 전 저널링. 크래시 후 재부팅 시 로그로 하이브를 복구. **리콘사일러(Reconciler)** 스레드가 더티 뷰와 안정적 뷰를 주기적으로 동기화.

### 어플리케이션 하이브 (Windows 8~)

`RegLoadAppKey` API로 앱 전용 프라이빗 하이브를 로드. AppContainer의 Settings.dat가 대표 예시.

### TxR (Transactional Registry)

KTM(커널 트랜잭션 관리자) + CLFS(Common Log File System) 기반 레지스트리 트랜잭션. `RegCreateKeyTransacted`/`RegOpenKeyTransacted`로 커밋·롤백 가능. NTFS TxF와 같은 트랜잭션으로 묶기 가능.

### 레지스트리 가상화

레거시 앱이 HKLM\Software 같은 전역 위치에 쓰려 할 때 `HKCU\Software\Classes\VirtualStore`로 리다이렉트. UAC와 함께 도입.

**차등 하이브(Differencing Hives)**: 기본 하이브 위에 변경 사항만 저장. 컨테이너 격리에 활용.

### 레지스트리 최적화

- **지연 플러시(Lazy Flushing)**: 변경을 모아서 기록
- **LRU 캐시**: 최근 접근 키 셀을 메모리에 캐시
- **하이브 재구성**: 삭제된 셀로 생기는 단편화 해소

---

## 10-2 Windows 서비스 (Windows Services)

### 서비스 개요

사용자 로그온 없이 자동 시작되는 백그라운드 프로세스. SCM(Service Control Manager, services.exe)이 생명주기를 관리.

### 서비스 구현 유형

| 유형 | 설명 |
|------|------|
| Win32 서비스 프로세스 | 독립 프로세스. SCM과 명명된 파이프로 통신 |
| 공유 서비스 프로세스 (Svchost) | 여러 서비스가 svchost.exe를 공유 |
| 커널 드라이버 서비스 | 드라이버를 SCM 구조로 관리 |

서비스 필수 구현 : `ServiceMain` + `RegisterServiceCtrlHandlerEx`

### 서비스 레지스트리 구성

`HKLM\SYSTEM\CurrentControlSet\Services\{서비스명}` 아래 설정.

| 값 | 의미 |
|---|------|
| Type | 1=커널 드라이버, 16=단독 프로세스, 32=공유 프로세스 |
| Start | 0=Boot, 1=System, 2=Automatic, 3=Manual, 4=Disabled |
| ErrorControl | 0=Ignore, 1=Normal, 2=Severe, 3=Critical |
| ObjectName | 실행 계정 |

### 서비스 계정 유형

| 계정 | 설명 |
|------|------|
| LocalSystem | 가장 강력. SYSTEM SID. 네트워크 접근 시 컴퓨터 계정 사용 |
| NetworkService | 제한적. 네트워크 접근 시 컴퓨터 계정 사용 |
| LocalService | 가장 제한적. 네트워크 접근 시 익명 사용 |
| 가상 서비스 계정 (NT SERVICE\{서비스명}) | 서비스별 격리 계정. 도메인 불필요 |
| Managed Service Account (MSA) | 도메인 연동 자동 비밀번호 관리 |
| Group Managed Service Account (gMSA) | 여러 서버에서 공유 가능한 MSA |

**서비스 SID**: 각 서비스에 NT SERVICE\{서비스명} 형식의 고유 SID. 파일·레지스트리 ACL에 서비스 SID를 설정해 서비스 간 격리.

### SCM (Service Control Manager)

services.exe. **SCM 데이터베이스**를 메모리에 유지하며 서비스 시작·중지·일시 중단·재개를 처리.

**서비스 자동 시작 순서**: `ServiceGroupOrder` 키의 그룹 순서 → 그룹 내 `Tag` 값 → 의존성 그래프 순서

**지연 자동 시작(Delayed Autostart)**: 부팅 후 일정 시간 후 시작. 부팅 시간 단축 목적.

**트리거 시작(Triggered-Start)**: ETW 이벤트, 장치 도착/제거, 도메인 참여, 네트워크 변경 등 발생 시 자동 시작.

**부팅 수락과 마지막 정상 구성(Last Known Good)**: 서비스 시작 오류가 Severe/Critical이면 마지막 정상 구성으로 재부팅 시도.

### 서비스 실패 복구

첫 번째·두 번째·이후 실패 시 동작을 각각 지정 가능: None / Restart / Run Program / Reboot.

### 공유 서비스 프로세스 (Svchost)

RAM 3.5GB 이상 Windows 10에서는 서비스별 개별 svchost 인스턴스. 이하는 공유.

**서비스 태그(Service Tag)**: TEB의 SubProcessTag 필드. 공유 svchost 안에서 스레드·소켓을 서비스에 매핑 (netstat -b 활용).

### 유저 서비스, 패키지 서비스, 보호 서비스

- **유저 서비스**: 세션별 인스턴스. 이름 형식 `{서비스명}_{세션ID}`
- **패키지 서비스**: UWP 앱 패키지 안에 정의. `PackageName` 레지스트리 값 추가
- **보호 서비스**: PPL로 실행. `LaunchProtected` 레지스트리 값으로 설정

---

## 10-3 태스크 스케줄러와 UBPM

### Task Scheduler

XML 서술자 기반 백그라운드 작업 스케줄러. `taskschd.msc` / `schtasks.exe`로 관리.

주요 구성 요소: Triggers(시간·이벤트·부팅·로그온·유휴 등) + Actions(프로그램 실행) + Conditions(추가 조건) + Settings(재시도·기간 한도) + Principal(실행 계정·권한)

COM 인터페이스(ITaskService, ITaskFolder, IRegisteredTask 등)로 프로그래밍 제어 가능.

### UBPM (Unified Background Process Manager)

Windows 8~. 다양한 백그라운드 작업 소스를 통합 관리.

| 컴포넌트 | 역할 |
|---------|------|
| UBPM | 시스템 전체 백그라운드 작업 조율 |
| SEB (System Events Broker) | 시스템 이벤트를 UBPM에 전달 |
| BI (Background Broker Infrastructure) | UWP 앱 백그라운드 트리거 처리 |
| Time Broker | 타이머 기반 트리거 관리 |

Modern Standby에서 UBPM이 백그라운드 작업을 지연·조절해 배터리 절약.

---

## 10-4 WMI (Windows Management Instrumentation)

### WMI 아키텍처

```
관리 앱 (PowerShell, C# 등)
    ↓ COM (IWbemServices 등)
WMI 서비스 (winmgmt.exe)
    ├── WMI 리포지터리 (%SystemRoot%\System32\wbem\Repository)
    └── WmiPrvSE.exe (프로바이더 호스트)
            ↓
        WMI 프로바이더 DLL
```

### CIM / MOF / WQL

- **CIM**: DMTF 정의 객체지향 정보 모델. Win32_Process, Win32_Service 등 내장 클래스
- **MOF**: WMI 클래스 기술 언어. `.mof` 파일 → `mofcomp.exe`로 등록
- **WQL**: `SELECT * FROM Win32_Process WHERE Name = 'notepad.exe'` 형식 쿼리

### WMI 프로바이더 유형

인스턴스 프로바이더 / 메서드 프로바이더 / 이벤트 프로바이더 / 속성 프로바이더

### WMI 보안

네임스페이스(root\cimv2 등)에 ACL 적용. `wmimgmt.msc`로 관리. 원격 접근은 DCOM 보안 설정 적용.

---

## 10-5 ETW (Event Tracing for Windows)

### ETW 3요소

**프로바이더**: GUID로 식별. `EventRegister` → `EventWrite` → `EventUnregister` API. ETW_REG_ENTRY가 커널에서 등록 상태 추적.

**세션**: 이벤트를 버퍼에 수집하는 채널. ETW Logger 스레드가 ETL 파일에 기록. `StartTrace` / `EnableTraceEx2`로 제어.

**컨슈머**: ETL 파일이나 실시간 스트림에서 이벤트 처리. WPA, PerfView, xperf 등.

### 전역 로거·오토로거

시스템 부팅 초기부터 추적 시작. `HKLM\SYSTEM\CurrentControlSet\Control\WMI\Autologger`에 설정 저장.

### ETW 보안

프로바이더 GUID별 보안 서술자로 구독 권한 제어.

### WPP / TraceLogging

- **WPP**: `DoTraceMessage` 매크로 기반. 오버헤드 매우 낮음. 드라이버·서비스에서 광범위 사용
- **TraceLogging**: Windows 10~. 사전 등록 없이 구조화된 이벤트 발행. 자동 스키마 포함

---

## 10-6 DTrace (Dynamic Tracing)

### 개요

Windows 10 1903~. 재컴파일·재부팅 없이 실행 중인 시스템을 동적으로 인스트루멘테이션.

### 내부 아키텍처

| 계층 | 구성 요소 |
|------|---------|
| 유저 모드 | dtrace.exe — D 스크립트 컴파일·커널 드라이버 통신 |
| 커널 모드 | DTrace.sys — 프로브 포인트 관리·안전성 검증 |
| 프로바이더 연동 | ETW 프로바이더를 DTrace 프로바이더로 노출 |

### D 스크립트 프로브 형식

`프로바이더:모듈:함수:이름`

```d
syscall:::entry
/execname == "notepad.exe"/
{
    printf("syscall: %s\n", probefunc);
}
```

내장 프로바이더: `syscall`, `pid`, `etw`, `profile`, `fbt`

DTrace 타입 라이브러리: EPROCESS, KTHREAD 등 Windows 내부 구조체 타입 내장. PDB 없이도 커널 구조체 필드 접근 가능.

---

## 10-7 WER (Windows Error Reporting)

### 유저 모드 앱 크래시

1. 미처리 예외 → `UnhandledExceptionFilter` 호출
2. `AeDebug` 키 디버거가 있으면 디버거 시작
3. WER 클라이언트(wermgr.exe)가 미니덤프 수집
4. 로컬 저장 후 Microsoft 전송
5. 해결책 있으면 Windows Action Center로 표시

**미니덤프 타입**: MiniDumpNormal(스택·모듈) / MiniDumpWithDataSegs(전역 데이터 포함) / MiniDumpWithFullMemory(전체 메모리)

**Fault Bucket**: 같은 버그를 분류·집계하는 ID.

### 커널 모드 크래시 (BSOD)

`KeBugCheckEx` 호출 → 덤프 생성 → WER 서비스가 재부팅 후 전송.

| 덤프 유형 | 설명 |
|---------|------|
| Minidump | 256KB. 스레드·스택 정보 |
| Kernel Memory Dump | 커널 모드 메모리만. 가장 일반적으로 유용 |
| Complete Memory Dump | 물리 메모리 전체 |
| Automatic Memory Dump | 커널 덤프 + 페이징 파일 자동 조정 |

**DPC Watchdog**: DISPATCH_LEVEL에서 과도한 실행 시간 → DPC_WATCHDOG_VIOLATION BSOD.

### 프로세스 중단 감지

HungAppTimeout(기본 5초) 후 메시지 없는 윈도우 감지 → 덤프 수집 → 재시작 권유 UI.

### Gflags.exe

전역 플래그 설정 도구. 힙 검증, 스택 추적 DB, 로더 스냅, 커널 특수 풀 등 활성화.

---

## 10-8 쉼 (Kernel Shims)

### 쉼 개요

레거시 앱의 호환성 문제를 앱 수정 없이 해결하는 메커니즘. 이미지 로더의 **쉼 엔진(Shim Engine)** 이 초기화 시 **SDB(Shim Database)** 를 조회해 IAT 패치·API 후킹으로 동작을 변경.

기본 SDB: `%SystemRoot%\AppPatch\sysmain.sdb`. 수천 개의 앱 호환성 항목 포함.

### 드라이버 쉼

PnP 관리자와 연계해 특정 하드웨어·드라이버 조합에 패치 적용. 드라이버 초기화 루틴 앞뒤에 래퍼 코드 삽입.

### 장치 쉼

USB·HID 장치의 비표준 동작을 표준 인터페이스로 변환. 장치 서술자 오류·보고 프로토콜 수정 등.

---

# 11. 캐싱과 파일 시스템 (Caching and File Systems)

---

## 11-1 캐시 관리자 (Cache Manager)

### 핵심 특성

- **단일 중앙 시스템 캐시**: OS 전체에 하나의 캐시 풀. 파일 시스템이 직접 캐시를 관리하지 않고 캐시 관리자에 위임
- **메모리 관리자 활용**: 캐시는 섹션 오브젝트(매핑 파일)로 구현. 물리 페이지 관리를 메모리 관리자에 완전히 위임
- **캐시 일관성**: 캐시된 뷰와 비캐시 I/O 경로가 같은 물리 페이지를 공유 → 자동 일관성
- **가상 블록 캐싱**: 파일 오프셋(VBO, Virtual Byte Offset) 기준으로 캐시
- **스트림 기반 캐싱**: 파일이 아닌 스트림(데이터 속성) 단위로 캐시
- **NTFS MFT 워킹셋 강화**: MFT 메타데이터를 우선적으로 캐시에 유지
- **메모리 파티션 지원**: 가상 머신별 독립 캐시 동작 가능

### 캐시 크기

| 크기 구분 | 설명 |
|---------|------|
| 캐시 가상 크기 | 섹션 뷰로 매핑된 가상 주소 범위 |
| 캐시 워킹셋 크기 | 현재 물리 메모리에 상주하는 캐시 페이지 |
| 캐시 물리 크기 | 캐시가 실제로 점유하는 물리 메모리 |

시스템 전체 가용 메모리에 따라 동적으로 조정.

### 캐시 데이터 구조

- **VACB(Virtual Address Control Block)**: 256KB 단위 캐시 슬롯. 파일 오프셋과 가상 주소 매핑을 추적
- **공유 캐시 맵(Shared Cache Map)**: 파일별 캐시 상태 추적. 파일 오브젝트가 포인팅
- **프라이빗 캐시 맵(Private Cache Map)**: 파일 핸들별 읽기 선행 상태 추적

### 파일 시스템 인터페이스

캐시 관리자는 파일 시스템에 세 가지 방식의 인터페이스를 제공:

| 방식 | 설명 |
|------|------|
| 복사 방식 (Copy) | CcCopyRead / CcCopyWrite. 유저 버퍼 ↔ 캐시 간 데이터 복사 |
| 매핑·핀 방식 (Mapping & Pinning) | CcMapData / CcPinRead / CcPinMappedData. 캐시 페이지를 직접 수정 |
| DMA 방식 (Direct Memory Access) | CcMdlRead / CcMdlWrite. MDL을 통한 직접 접근 |

---

## 11-2 읽기 선행과 지연 쓰기

### 지능적 읽기 선행 (Intelligent Read-Ahead)

순차 읽기 패턴을 감지하면 미리 다음 데이터를 읽어 캐시에 채움.

- 프라이빗 캐시 맵이 각 파일 핸들의 읽기 패턴을 추적
- 읽기 선행 크기를 동적으로 증가 (최대 캐시 맵 크기 한도)
- 역방향·비선형 읽기 패턴 감지 시 읽기 선행 비활성화

읽기 선행 강화(Read-Ahead Enhancements): 대용량 파일 순차 읽기 시 더 넓은 단위로 선행.

### 지연 쓰기 (Write-Back Caching / Lazy Write)

더티 페이지를 즉시 디스크에 기록하지 않고 일정 시간 후 배치 기록.

**캐시 관리자의 지연 쓰기 스레드**:

| 스레드 | 역할 |
|--------|------|
| 지연 쓰기 스레드 | 더티 VACB를 주기적으로 디스크에 플러시 |
| 읽기 선행 스레드 | 비동기 읽기 선행 I/O 수행 |

**강제 즉시 쓰기(Write-Through)**: `FILE_FLAG_WRITE_THROUGH`로 파일 열면 각 쓰기 시 즉시 플러시.

**쓰기 스로틀링(Write Throttling)**: 더티 페이지가 과도하게 쌓이면 쓰기 앱 스레드를 일시 차단해 시스템 응답성 유지.

**적극적 지연 쓰기와 저우선순위 지연 쓰기**: 메모리 압박 시 더 적극적으로 플러시. 저우선순위 쓰기로 포그라운드 I/O 방해 최소화.

---

## 11-3 Fast I/O

IRP를 생성하지 않고 파일 시스템 드라이버의 Fast I/O 루틴을 직접 호출하는 경로.

- 캐시 히트가 예상될 때 I/O 관리자가 먼저 시도
- 드라이버가 Fast I/O를 지원하지 않거나 즉시 처리 불가 시 → 일반 IRP 경로로 폴백
- 파일 시스템 필터 드라이버가 Fast I/O를 가로챌 수 있음
- NTFS, FAT 등 로컬 파일 시스템이 주로 활용

---

## 11-4 파일 시스템 종류

### FAT12 / FAT16 / FAT32

FAT(File Allocation Table) 기반. 단순한 구조. FAT32는 최대 4GB 파일 한도. exFAT은 이 한도를 제거한 확장판 (플래시 메모리 최적화).

### CDFS

CD-ROM 전용. ISO 9660 기반. 읽기 전용.

### UDF

CD/DVD/Blu-ray용. 읽기·쓰기 모두 지원.

### NTFS

Windows 기본 파일 시스템. 자세한 내용은 11-5 참조.

### ReFS (Resilient File System)

Windows Server 2012~. 자세한 내용은 11-6 참조.

### 파일 시스템 드라이버 아키텍처 (FSD)

```
I/O 관리자
    ↓ IRP
로컬 FSD (Ntfs.sys, Fat.sys 등)
    ↓ 스토리지 스택
    또는
원격 FSD (Mup.sys → SMB 리다이렉터 등)
    ↓ 네트워크 스택
```

**필터 드라이버와 미니필터**: Filter Manager(fltmgr.sys) 위에 미니필터 드라이버가 등록되어 파일 I/O를 가로챔. 백신, 암호화, 감사 드라이버 등이 활용.

**리파스 포인트(Reparse Point)**: 파일/디렉터리에 붙는 재해석 태그. 심볼릭 링크, 정션, 마운트 포인트, OneDrive 스텁 파일 등에 활용.

**오플락(Oplock, Opportunistic Lock)**: 원격 클라이언트가 파일을 로컬에 캐시할 수 있도록 허용하는 메커니즘. 서버가 다른 클라이언트의 충돌 접근을 감지하면 오플락을 파괴(break)해 클라이언트에게 캐시를 플러시하도록 알림.

---

## 11-5 NTFS (NT File System)

### 핵심 특성

- **복구 가능성**: LFS(Log File Service) 기반 메타데이터 로깅. 크래시 후 자동 복구
- **보안**: 파일·디렉터리에 ACL 적용. ACL을 안전하게 보관하는 통합 보안 스트림
- **대용량**: 64비트 클러스터 번호(LCN). 이론상 수십 EB 볼륨 지원
- **유니코드**: 모든 파일명을 유니코드로 저장

### NTFS 고급 기능

| 기능 | 설명 |
|------|------|
| 다중 데이터 스트림 | 파일에 여러 이름 붙은 데이터 스트림 첨부. 기본 스트림 외에 숨겨진 데이터 보관 가능 |
| 일반 인덱싱 | 파일 이름, OID 등 다양한 키로 B+ 트리 인덱스 구성 |
| 동적 불량 클러스터 재매핑 | 불량 클러스터 발견 시 스페어로 자동 교체 |
| 하드 링크 | 여러 파일 이름이 같은 파일 데이터를 가리킴 |
| 심볼릭 링크·정션 | 리파스 포인트 기반. 파일 또는 디렉터리 별칭 |
| 변경 저널(Change Journal, USN) | 파일 변경을 USN(Update Sequence Number) 저널에 기록. 백업·검색 엔진 활용 |
| 사용자별 디스크 할당량 | 볼륨 내 사용자별 디스크 사용량 추적·제한 |
| 링크 추적 | 파일이 이동되거나 이름이 바뀌어도 링크 유지 |
| EFS 암호화 | 파일 데이터를 투명하게 암호화. 인증서 기반 |
| POSIX 삭제 시맨틱 | 열려 있는 파일을 삭제해도 열린 핸들이 닫힐 때까지 데이터 유지 |
| 조각 모음 | 온라인 상태에서 파일 재배치 가능 |
| 동적 파티셔닝 | 볼륨 확장·축소 |
| 계층 볼륨 지원 | SSD·HDD 계층 간 자동 파일 이동(Storage Tiers) |
| 스토리지 예약 | 특정 앱이 디스크 공간을 예약 |
| 트랜잭션 지원 (TxF) | KTM 기반 트랜잭션 파일 I/O |

### NTFS 온디스크 구조

| 구성 요소 | 설명 |
|---------|------|
| 볼륨 부트 레코드(VBR) | 첫 섹터. 파일 시스템 파라미터(BPB) 포함 |
| MFT (Master File Table) | 볼륨의 모든 파일·디렉터리 레코드 배열. 첫 16개는 메타데이터 파일 |
| 클러스터 (LCN) | 할당 최소 단위. 기본 4KB |
| 파일 레코드 | 기본 1KB. 파일 속성(이름, 데이터, 보안, 인덱스 등)을 포함 |
| 거주 속성 (Resident) | 데이터가 파일 레코드 안에 직접 저장 (소용량 파일) |
| 비거주 속성 (Non-Resident) | 데이터가 클러스터에 저장. 파일 레코드는 Run List만 보유 |
| 희소 파일 (Sparse File) | 비어있는 범위를 디스크에 할당하지 않아 공간 절약 |
| 압축 파일 | 16클러스터 단위로 LZ 압축 |

### NTFS 복구 지원

**LFS(Log File Service)**: NTFS 메타데이터 변경을 원형 로그 파일($LogFile)에 사전 기록(Write-Ahead Logging).

**복구 3단계**:
1. **분석 패스(Analysis Pass)**: 마지막 체크포인트 이후 커밋·미커밋 트랜잭션 파악
2. **리두 패스(Redo Pass)**: 커밋된 트랜잭션 재적용
3. **언두 패스(Undo Pass)**: 미커밋 트랜잭션 롤백

**셀프 힐링(Self-Healing)**: 온라인 상태에서 오류 감지 → 투명 복구. `chkdsk` 없이 자동 처리.

**온라인 chkdsk / 빠른 복구**: Windows 8~. 서비스 중단 없이 메타데이터 오류 감지·복구.

### EFS (Encrypting File System)

파일 데이터를 투명하게 암호화하는 NTFS 기능.

**암호화 흐름**:
1. 파일을 열 때 FEK(File Encryption Key, 대칭 키)로 복호화
2. FEK는 사용자 인증서의 공개 키로 암호화되어 $EFS 속성에 저장
3. 복호화 시 사용자 개인 키로 FEK 복호화

BitLocker와의 차이: EFS는 파일 단위 암호화(사용자 인증서 기반) / BitLocker는 볼륨 전체 암호화(TPM·PIN 기반).

**온라인 암호화 지원**: Windows 10~. 파일이 열려있는 동안 비동기로 암호화/복호화.

### DAX 디스크 (Direct Access Disks)

NVM(Non-Volatile Memory, 예: Intel Optane)을 파일 시스템 캐시를 우회하고 직접 접근하는 기능.

- **DAX 드라이버 모델**: SCM(Storage Class Memory) 스토리지 드라이버 모델
- **DAX 볼륨**: NTFS 포맷 볼륨에서 DAX 모드 활성화 시 캐시 관리자 바이패스
- **영구 메모리(PM)**: 전원이 꺼져도 데이터를 유지하는 바이트 주소 지정 가능 스토리지
- **대형·초대형 페이지 지원**: DAX 볼륨에서 2MB, 1GB 페이지로 매핑 가능
- `FILE_FLAG_DAX_VOLUME`으로 생성된 섹션은 캐시 관리자 없이 NVM에 직접 매핑

---

## 11-6 ReFS (Resilient File System)

### ReFS 설계 목표

- **데이터 무결성**: 모든 메타데이터와 선택적으로 데이터에 체크섬 적용. 읽기 시 자동 검증
- **대용량**: 파일·볼륨 크기 제한 사실상 없음 (요르)
- **Storage Spaces와 통합**: 패리티·미러 스페이스와 협력해 자동 복구
- **쓰기 전 로깅**: 원자성 있는 메타데이터 업데이트

### Minstore 아키텍처

ReFS의 스토리지 엔진. B+ 트리를 중심으로 구성된 내부 레이어.

| 구성 요소 | 역할 |
|---------|------|
| B+ 트리 | 메타데이터·데이터를 16KB 페이지 단위 B+ 트리로 관리 |
| 할당자 (Allocator) | 오브젝트 테이블·컨테이너 테이블·스키마 테이블로 구성. 공간 할당 관리 |
| 페이지 테이블 | 논리 블록 주소(LBA)를 실제 디스크 블록으로 변환 |

**Minstore 쓰기 전 로깅**: 모든 변경을 원형 로그에 사전 기록 후 B+ 트리 업데이트.

**Minstore I/O**: 비동기 I/O 완료 콜백 기반. 읽기·쓰기 I/O를 배치 처리.

### ReFS 온디스크 구조

- **오브젝트 ID**: 모든 B+ 트리 오브젝트에 64비트 ID 부여. 오브젝트 테이블로 물리 주소 참조
- **보안 및 변경 저널**: NTFS와 유사하게 ACL, USN 저널 지원
- **트랜잭션 로그**: 메타데이터 변경의 원자성 보장

### ReFS 고급 기능

| 기능 | 설명 |
|------|------|
| 블록 클로닝 (Block Cloning) | 파일 블록 복사 시 실제 데이터 복사 없이 오브젝트 ID 공유. VM/컨테이너 이미지 복사 성능 극대화 |
| 희소 VDL (Sparse VDL) | VDL(Valid Data Length)을 활용해 초기화 없이 빠른 스파스 파일 확장 |
| ReFS 쓰기 스루 (Write-Through) | 트랜잭션 로그 기반 쓰기 전 확인. 데이터 손실 방지 |
| SMR (Shingled Magnetic Recording) 지원 | 순차 쓰기 최적화가 필요한 SMR 디스크와 협력 |
| 계층 볼륨 지원 | Storage Spaces와 통합해 성능·용량 계층 자동 관리 |
| 컨테이너 압축 | 데이터 블록을 논리 컨테이너로 묶어 실시간 압축 |
| 고스팅 | 자주 안 쓰는 데이터를 느린 계층으로 자동 이동 |
| 누출 감지 | 고아 블록 추적·회수 |

### ReFS 복구 지원

- 메타데이터 변경 시 쓰기 전 로깅으로 원자성 보장
- Storage Spaces의 미러·패리티 정보로 손상된 데이터 자동 복구
- 자기 복구(Self-Healing): 체크섬 오류 감지 → 미러 복사본으로 투명 복구

---

## 11-7 Storage Spaces (스토리지 공간)

### 개요

여러 물리 디스크를 풀링해 가상 디스크(스페이스)를 제공하는 소프트웨어 기반 스토리지 관리 기술.

### Spaces 내부 아키텍처

```
앱 / 파일 시스템 (NTFS, ReFS)
    ↓ IRP
Storage Spaces 드라이버 (spaceport.sys)
    ↓ 가상 디스크 (스페이스)
    ↓ I/O 분산
물리 디스크 풀 (여러 드라이브)
```

### Spaces가 제공하는 서비스

| 기능 | 설명 |
|------|------|
| 단순 (Simple) | RAID 0과 유사. 스트라이핑. 속도 우선, 중복 없음 |
| 미러 (Mirror) | RAID 1/5와 유사. 2-방향 또는 3-방향 미러링. 드라이브 장애 허용 |
| 패리티 (Parity) | RAID 5/6과 유사. LRC(Local Reconstruction Codes) 또는 RAID 6 패리티. 공간 효율적 중복 |
| 계층 (Tiered) | SSD(고성능)와 HDD(대용량) 계층 자동 관리. 자주 사용하는 데이터를 SSD에 유지 |
| SCM 지원 | NVM 스토리지를 Storage Spaces 풀에 추가 가능 |

---

# 12. 시작과 종료 (Startup and Shutdown)

---

## 12-1 부팅 프로세스 (Boot Process)

### UEFI 부팅

```
전원 ON → UEFI 펌웨어
    → UEFI POST (Power-On Self Test)
    → UEFI 부팅 관리자 (NVRAM에서 부팅 항목 로드)
    → Windows 부팅 관리자 (bootmgfw.efi)
    → Windows OS 로더 (winload.efi)
    → Windows 커널 (ntoskrnl.exe)
```

### BIOS 부팅 (레거시)

```
전원 ON → BIOS POST
    → MBR(마스터 부트 레코드) 로드
    → 파티션 부트 섹터 로드
    → Windows 부팅 관리자 (bootmgr)
    → Windows OS 로더 (winload.exe)
    → Windows 커널
```

### UEFI vs BIOS 비교

| 특성 | UEFI | BIOS |
|------|------|------|
| 아키텍처 | 32/64비트 독립 모듈 | 16비트 어셈블리 |
| 파티션 | GPT 지원 | MBR만 지원 |
| 보안 부팅 | Secure Boot 내장 지원 | 지원 불가 |
| 디스크 크기 | 2TB 이상 지원 | 2TB 이하 |
| EFI 앱 | .efi 실행 파일 지원 | 미지원 |

---

## 12-2 Secure Boot (보안 부팅)

UEFI 펌웨어 레벨에서 부팅 경로의 모든 코드를 서명으로 검증하는 메커니즘.

### 신뢰 체인 (Chain of Trust)

```
UEFI 펌웨어 (KEK 포함)
    → Windows 부팅 관리자 (Microsoft 서명)
    → Windows OS 로더 (Microsoft 서명)
    → Windows 커널 (Microsoft 서명)
    → 드라이버 (KMCS 정책)
```

- **PK(Platform Key)**: 플랫폼 제조사의 최상위 서명 키
- **KEK(Key Exchange Key)**: 허용 서명 DB(db)와 거부 DB(dbx) 업데이트를 허가하는 키
- **db**: 부팅 허용 서명 목록
- **dbx**: 부팅 거부 서명 목록 (취약점이 발견된 부트로더 차단)

### Secure Boot 우회 방지

취약한 부트로더가 발견되면 Microsoft가 dbx를 업데이트해 해당 서명을 차단. BitLocker와 함께 사용 시 TPM이 부팅 측정값을 봉인(seal)해 변조 감지.

---

## 12-3 Windows 부팅 관리자 (Windows Boot Manager)

UEFI 환경: `bootmgfw.efi` / BIOS 환경: `bootmgr`

### BCD (Boot Configuration Data)

부팅 옵션을 저장하는 구조화된 데이터베이스. `bcdedit.exe`로 관리.

**BCD 오브젝트 유형**:

| 유형 | 설명 |
|------|------|
| 부팅 관리자 오브젝트 | Windows 부팅 관리자 자체 설정 |
| OS 로더 오브젝트 | 각 Windows 설치 항목 |
| 부팅 앱 오브젝트 | WinRE, 메모리 테스트 앱 등 |
| 상속 오브젝트 | 다른 오브젝트에 상속되는 공통 설정 |

### 부트 메뉴

운영 체제가 여러 개 설치된 경우 선택 메뉴 표시. 타임아웃 후 기본 OS 자동 로드.

---

## 12-4 측정 부팅 (Measured Boot)

TPM(신뢰 플랫폼 모듈)을 활용해 부팅 과정의 모든 단계를 순서대로 측정(해시)하고 TPM의 PCR(Platform Configuration Register)에 기록하는 기능.

### 측정 순서

```
UEFI 펌웨어 측정 → BCD 측정 → 부팅 관리자 측정
→ OS 로더 측정 → 커널 측정 → 드라이버 측정
→ 모든 측정값이 TPM PCR에 누적
```

### TPM 봉인(Sealing)

BitLocker는 TPM PCR 값이 예상값과 일치할 때만 암호화 키를 해제. 부팅 경로가 변조되면 PCR 값이 달라져 키 해제 실패 → 드라이브 잠금.

### 원격 증명(Remote Attestation)

네트워크 서버가 클라이언트의 TPM PCR 값을 요청해 부팅 무결성을 원격으로 검증. 조건부 접근 제어에 활용.

**TPM 측정값 무효화**: 업데이트 설치 등 변경 후 예상 측정값을 재산정하지 않으면 BitLocker가 복구 키를 요구.

---

## 12-5 신뢰 실행 (Trusted Execution)

### TBOOT (Measured Launch / Trusted Boot)

Intel TXT(Trusted Execution Technology) 기반. 부팅 전에 하이퍼바이저를 안전하게 시작하는 DRTM(Dynamic Root of Trust Measurement) 메커니즘.

### Secure Launch (DRTM)

Windows 10 1809~. SKINIT(AMD) 또는 TXT(Intel)를 사용해 동적으로 새로운 신뢰 루트를 확립.

부팅 과정에 이미 악성 코드가 있어도 Secure Launch가 무결한 상태에서 hypervisor와 VBS를 시작할 수 있음.

---

## 12-6 Windows OS 로더 (Winload)

UEFI 환경: `winload.efi` / BIOS 환경: `winload.exe`

### 주요 역할

1. NTFS/ReFS에서 OS 파일 로드 (커널, HAL, 부트 드라이버)
2. UEFI 서비스를 통해 물리 메모리 맵 수집
3. 로더 파라미터 블록(Loader Parameter Block) 구성 → 커널에 전달
4. 페이지 테이블 구성 (초기 가상 주소 매핑)
5. 코드 서명 검증 (부트 드라이버 포함)
6. 하이퍼바이저 로더(HvLoader) 시작 (Hyper-V 활성화 시)
7. Secure Kernel 로드 (VBS 활성화 시)

### 로더 파라미터 블록

Winload가 커널에 전달하는 부팅 파라미터 구조체. 다음을 포함:
- 물리 메모리 맵
- 로드된 모듈 목록 (커널, HAL, 드라이버)
- NUMA 토폴로지
- UEFI 런타임 서비스 포인터
- 펌웨어 제공 테이블 (ACPI, SMBIOS 등)

### 하이퍼바이저 로더 (HvLoader)

Winload가 Hyper-V 활성화를 감지하면 HvLoader.dll을 로드. HvLoader가 CPU 제조사에 맞는 하이퍼바이저 바이너리(Hvix64.exe / Hvax64.exe / Hvaa64.exe)를 선택해 시작.

### VSM 시작 정책

VBS 활성화 여부는 BCD와 UEFI 펌웨어 설정의 조합으로 결정. `LOADOPTIONS` BCD 항목에 `HYPERVISORLAUNCHTYPE` 포함.

---

## 12-7 커널과 Executive 초기화

### Phase 0 (단일 스레드)

최소한의 커널 환경을 구성. IDT, GDT 설정, 기본 메모리 관리자 초기화, HAL 초기화.

### Phase 1 (다중 프로세서 초기화 전까지)

Phase 1에서 초기화되는 주요 컴포넌트들 (순서대로):

| 구성 요소 | 역할 |
|---------|------|
| HAL | 하드웨어 추상화 계층 완전 초기화 |
| 커널 디버거 | KdInitSystem — 디버거 연결 준비 |
| 메모리 관리자 | MmInitSystem Phase 1 — 페이지드·논페이지드 풀 설정 |
| 오브젝트 관리자 | ObInitSystem — 오브젝트 네임스페이스 루트 생성 |
| 보안 참조 모니터 | SeInitSystem — 토큰, ACL 처리 준비 |
| 프로세스 관리자 | PsInitSystem — 시스템·유휴 프로세스 생성 |
| PnP 관리자 | 드라이버 로드 인프라 준비 |
| I/O 관리자 | 드라이버 로드 시작 (부트 드라이버 → 시스템 드라이버) |
| 세션 관리자 (Smss) | 첫 번째 유저 모드 프로세스로 생성 |

### ReadyBoot

시스템 시작 후 첫 120초 이내의 파일 I/O 패턴을 추적해 다음 부팅 시 해당 파일들을 미리 읽어오는 기능. SuperFetch의 부팅 최적화 버전.

---

## 12-8 Smss, Csrss, Wininit

### Smss (Session Manager)

커널이 직접 생성하는 첫 번째 유저 모드 프로세스.

**역할**:
1. 레지스트리 `BootExecute` 항목 실행 (autochk.exe 등)
2. 페이징 파일 생성 (`PagingFiles` 레지스트리 값)
3. 레지스트리 하이브 로드
4. 서브시스템(csrss.exe) 시작
5. Wininit.exe 시작
6. 세션 1+ 생성을 위한 Smss 복사본 시작 (병렬)

### Csrss (Client/Server Runtime Subsystem)

Win32 서브시스템 프로세스.

- Win32 콘솔 창 관리, 프로세스·스레드 추적, Raw Input Thread 보유
- 세션당 하나씩 실행 (세션 0 + 각 로그온 세션)

### Wininit (Windows Initialization)

세션 0 초기화 프로세스.

**역할**:
1. 세션 0 윈도우 스테이션 설정
2. Services.exe (SCM) 시작
3. Lsass.exe 시작
4. Lsm.exe (Local Session Manager) 시작

---

## 12-9 자동 시작 이미지

부팅 완료 후 자동으로 시작되는 항목들의 처리 순서:

1. 서비스 (SCM의 Autostart 서비스)
2. `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` 실행 키
3. `HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` 실행 키
4. 시작 프로그램 폴더
5. 태스크 스케줄러 자동 시작 작업

---

## 12-10 종료 (Shutdown)

### 정상 종료 흐름

```
사용자 종료 요청 → csrss.exe → 포그라운드 앱에 WM_QUERYENDSESSION 전송
    → 앱 응답 대기 (HungAppTimeout)
    → WM_ENDSESSION으로 종료 확인
    → 서비스들에 SERVICE_CONTROL_STOP 전송
    → NtShutdownSystem → Phase 1 종료 시작
    → 파일 시스템 플러시 → 드라이버 종료
    → 전원 차단 또는 재시작
```

### 하이브리드 종료 (Hybrid Shutdown, Windows 8~)

사용자 세션은 정상 종료(로그오프)하지만 커널 세션은 최대절전으로 저장.

다음 부팅 시 커널 세션이 Hiberfil.sys에서 복원되어 빠른 시작.

`shutdown /s /hybrid` 또는 Fast Startup 설정으로 활성화.

---

## 12-11 최대절전과 빠른 시작 (Hibernation and Fast Startup)

### 최대절전 (Hibernation, S4)

물리 메모리 전체를 Hiberfil.sys에 저장 후 전원 차단.

**Hiberfil.sys 생성**: Memory Manager가 모든 커널 페이지를 Hiberfil.sys에 순서대로 기록.

**복원**: Winload.exe 대신 `winresume.exe`/`winresume.efi`가 Hiberfil.sys를 읽어 메모리 상태를 복원.

### 빠른 시작 (Fast Startup)

최대절전의 변형. 사용자 세션은 정상 로그오프하고 커널 세션만 Hiberfil.sys에 저장.

복원 시간이 완전 부팅보다 훨씬 빠름. 드라이버 초기화와 커널 Phase 0/1을 건너뜀.

**주의**: 빠른 시작 활성화 시 NTFS 볼륨이 정상적으로 마운트 해제되지 않아 Linux 듀얼 부팅에서 문제 발생 가능.

---

## 12-12 Windows 복구 환경 (WinRE)

### WinRE 개요

Windows가 정상 부팅되지 않을 때 실행되는 복구 환경. 별도 파티션(WinRE 파티션) 또는 WIM 파일로 제공.

**복구 옵션**:
- 시스템 복원 (System Restore)
- 시스템 이미지 복구
- 시작 복구 (Startup Repair)
- 명령 프롬프트
- 시스템 초기화 (PC Reset)

### 부팅 실패 시 자동 진입

WinBoot가 연속 부팅 실패를 감지하면 자동으로 WinRE로 진입.

---

## 12-13 안전 모드 (Safe Mode)

Windows가 최소한의 드라이버·서비스만 로드해 시작하는 진단 모드.

### 드라이버 로드 규칙

`SafeBoot` 레지스트리 키(`HKLM\SYSTEM\CurrentControlSet\Control\SafeBoot`)에 등록된 드라이버·서비스만 로드.

| 안전 모드 유형 | 설명 |
|-------------|------|
| 안전 모드 (Minimal) | 최소 드라이버·서비스만 로드. 네트워크 없음 |
| 안전 모드 (네트워킹 포함) | 네트워크 드라이버 추가 로드 |
| 안전 모드 (명령 프롬프트) | Explorer 대신 cmd.exe 실행 |

### 안전 모드 인식 프로그램

`GetSystemMetrics(SM_CLEANBOOT)` API로 안전 모드 여부 확인 가능. 안전 모드에서는 불필요한 기능을 비활성화하거나 관리자만 허용하도록 동작 변경.
