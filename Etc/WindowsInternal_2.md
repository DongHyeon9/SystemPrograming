# 8. 시스템 메커니즘 (System Mechanisms)

---
 
## 8-1 프로세서 실행 모델 (Processor Execution Model)
 
### 세그멘테이션 (Segmentation)
 
x86/x64 CPU는 두 가지 메모리 보호 모델을 지원한다
 
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
