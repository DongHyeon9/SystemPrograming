Windows Intenal 부록 : https://aka.ms/winint7ed/downloads

소스코드 : https://github.com/zodiacon/windowsinternals

# 1. 개념과 툴
## 1-1 운영체제 버전
커맨드라인 ver명령어 혹은 winver를 실행해 윈도우 버전 정보를 볼 수 있다 -> 빌드번호를 통해 어떤 패치가 이뤄졌는지 확인 가능(보안 업데이트 관리에 도움)
## 1-2 윈도우 API
윈도우 API는 C언어로 만들어져 있다(당시 환경에 의한 자연스러운 선택)

함수명의 일관성 결여, 논리적 그룹화(ex. C++ 네임스페이스)의 부족, 연관된 함수의 수적인 측면 등의 단점으로 C유형과는 상이한 API 매커니즘인 COM(Component Object Model)을 제작

COM은 마이크로소프트 오피스 어플리케이션에서 문서 간의 데이터 통신과 교환(엑셀차트 -> 워드문서 or ppt프레젠테이션으로 삽입 등)을 위해 고안됨

객체의 연결과 삽입 = OLE(Object Linking and Embedding) -> 동적 데이터 교환(DDE, Dynamic Data Exchange) 메커니즘을 사용해 구현 -> 본질적인 제약이 있어 COM을 개발

COM 1. 클라이언트는 인터페이스를 통해 객체와 통신한다 -> C, C++, 비주얼베이직, .NET 등 메소르를 호출할 수 있도록 한다

COM 2. 컴포넌트 구현 부분이 클라이언트에 정적으로 링크되는 것이 아닌 동적으로 로드됨

COM 서버 = *.DLL or *.exe 

COM은 보안, 크로스프로세스 마샬링, 스레드 모델등과 연관된 중요한 특성이 있음 (추천 도서 Essential COM(Addison-Wesley, 1998))

## 1-3 서비스, 함수, 루틴
### 윈도우 API 함수
윈도우 API에 문서화돼 있고 호출 가능한 서브루틴 -> ex. CreateProcess, CreateFile, GetMessage 등
### 네이티브 시스템 서비스(System Call)
운영체제 하부에 있는 문서화 되지 않은 서비스, 유저모드에서 호출 가능 -> ex. NtCreate-UserProcess = CreateProccess함수가 새로운 프로세스를 생성하기 위해 호출하는 내부 시스템 서비스
### 커널 지원 함수(Routine)
커널 모드에서만 호출 가능한 윈도우 운영체제 내부의 서브루틴 -> ExAllocatePool-WithTag = 디바이스 드라이버가 윈도우 시스템 힙(pool이라고 부른다)으로부터 메모리를 할당하기 위해 호출하는 루틴
### 윈도우 서비스
윈도우 서비스 컨트롤 관리자에 의해 시작된 프로세스

## 1-4 프로세스
저장된건 프로그램 그걸 실행해서 인스턴스화하면 프로세스
### 구성요소
#### 전용 가상 주소 공간
VMS, virtual Memory Space
#### 실행 가능한 프로그램
초기 코드, 데이터 -> 프로세스의 VMS에 매핑됨
#### 오픈 핸들 목록
프로세스 내의 모든 스레드에서 접근 가능한 세마포어, 이벤트, 파일 같은 다양한 시스템 리소스
#### 보안 컨텍스트
해당 앱 컨테이너식별자와 이에 관련된 샌드박스정보, 프로세스의 보안그룹, 권한, 속성, 클레임, 기능, 사용자 계정 컨트롤 가상화 상태, 세션, 제환된 사용자 계정 상태를 식별
#### 프로세스 ID
프로세스의 고유한 ID (PID)
#### 최소 하나의 실행 스레드
main용 스레드

## 1-5 스레드
실행을 위해 스케줄링하는 프로세스 내의 개체
### 구성요소
#### 프로세서의 상태를 표현하는 CPU 레지스터 집합의 내용
#### 커널모드용 스택, 유저모드용 스택
#### TLS
#### TID
#### 보안컨텍스트(토큰)
### 스레드 컨텍스트
휘발성 레지스터와 스택, TLS를 일컫는 말

이 정보는 윈도우가 실행되는 각 머신의 아키텍처에 따라 다르다( GetThreadContext 함수를 사용해 CONTEXT 블록이라고 부르는 아키텍처 한정적 정보에 접근 가능 )
### 컨텍스트 스위칭
한 스레드에서 다른 스레드로의 실행 전환은 커널 스케줄러의 관여가 이뤄지므로 비싼동작, 윈도우는 이 비용을 절감하기 위해 파이버(fiber)와 유저모드 스케줄링(UMS, User Mode Scheduling)이라는 두가지 메커니즘을 구현
#### 파이버(fiber)
윈도우에 내장된 우선순위 기반 스케줄링 메커니즘에 의존하지 않고 자신의 실행 스레드를 스케줄링 하는것을 허용( 종종 경량 스레드라고 불린다 )

유저모드인 Kernel32.dll에 구현되어 있음

ConvertThreadToFiber 함수를 호출해서 사용 가능 -> 실행중인 스레드를 파이버로 변환 -> 이후 파이버는 CreateFiber함수를 통해 파이버 생성 가능

스레드와 달리 파이버는 SwitchToFiber 함수를 호출해 수동으로 선택되기 전에는 실행되지 않는다

파이버는 커널에 보이지 않기 때문에 사용에 있어서 신중해야된다
#### 유저모드 스케줄링 스레드(UMS, User Mode Scheduling)
파이버가 가진 기본적인 장점들을 제공

UMS는 커널에 보인다

## 1-6 그 외 구성요소
### 하이퍼바이저
가상 메모리에서 물리 메모리의 장치 인터럽트, PCI와 USB 장치까지 머신의 모든 자원을 가상화, 격리시켜주는 특화되고 매우 특권이 높은 구성 요소

매우 높은 권한, 커널보다 더 많은 접근을 가지므로 다른 운영체제의 여러 게스트 인스턴스를 실행하는 것을 넘어서는 장점( ex. 하이퍼-V )
### 펌웨어
부트 과정을 보장할 수 있는 신뢰성 연결 고리의 기본을 제공

### 레지스트리
부트와 시스템 설정, 동작을 제어하는 시스템 전역적인 소프트웨어 설정, 보안 데이터베이스, 스크린 세이버 사용같은 사용자별 설정을 담고있는 시스템 데이터베이스

윈도우 성능 카운터와 마찬가지로 현재 하드웨어의 상태같은 메모리에 있는 휘발성 데이터를 볼 수 있다

## 1-7 툴
### 성능 모니터 : 기본 프로그램, GUI 기반의 성능 체크용 ( 시스템 감시와 성능 카운터 로그 조회, 알림 설정 등)
VMS, virtual Memory Space
#### 실행 가능한 프로그램
초기 코드, 데이터 -> 프로세스의 VMS에 매핑됨
#### 오픈 핸들 목록
프로세스 내의 모든 스레드에서 접근 가능한 세마포어, 이벤트, 파일 같은 다양한 시스템 리소스
#### 보안 컨텍스트
해당 앱 컨테이너식별자와 이에 관련된 샌드박스정보, 프로세스의 보안그룹, 권한, 속성, 클레임, 기능, 사용자 계정 컨트롤 가상화 상태, 세션, 제환된 사용자 계정 상태를 식별
#### 프로세스 ID
프로세스의 고유한 ID (PID)
#### 최소 하나의 실행 스레드
main용 스레드
#### Dependency Walker
DLL 의존성 확인
#### PEview
exe 파일의 구성요소를 확인
#### ProcessExplorer
프로세스의 상세한 정보 ( dll, PID 등)

# 2. 시스템 아키텍처
## 2-1 커널모드 컴포넌트
### Excutive
메모리, 프로세스, 스레드 관리, 보안, I/O, 네트워킹, 프로세스 간 통신 ( IPC, Inner Process Comunication )과 같은 기본 OS 서비스
### 윈도우 커널
스레드 스케줄링, 인터럽트, 예외 디스패칭, 멀티프로세서 동기화 등 저수준 OS 함수로 구성

익스큐티브의 나머지 부분이 상위 구성체를 구현하기 위해 사용하는 일련의 루틴과 기본 객체 제족
### 디바이스 드라이버
유저 I/O 함수 호출을 특정 하드웨어 디바이스 I/O 요청으로 변환하는 파일시스템, 네트워크 드라이버 등 하드웨어와 비하드웨어 드라이버 양쪽 모두를 포함
### 하드웨어 추상화 계층 ( HAL, Hardware Abstraction Layer )
커널, 디바이스 드라이버, 기타 윈도우 익스큐티브의 나머지 부분을 플랫폼마다 다른 하드웨어의 차이점(마더보드 간의 상이점)으로부터 분리시켜주는 계층의 코드
### 윈도잉과 그래픽 시스템
GUI함수(윈도우 User와 GDI 함수)구현
### 하이퍼바이저 계층
하이퍼바이저 자신만의 단일 컴포넌트로 구성, 드라이버나 다른 모듈이 존재하지 않음

하이퍼바이저는 자체적인 메모리관리자, 가상프로세스 스케줄러, 인터럽트와 타이머관리 동기화 루틴 등과 같은 다수의 내부 계층과 서비스로 이뤄짐
## 2-2 핵심 윈도우 시스템 파일
 - Nioskrnl.exe : 익스큐티브와 커널
 - hal.dll : HAL
 - Win32k.sys : 윈도우 서브시스템의 커널 모드 부분(GUI)
 - Hvis64.exe(Intel), Hvax64.exe(AMD) : 하이퍼바이저
 - \systemRoot\System32\Drivers 내의 *.sys 파일 : DX와 볼륨 관리자, TCP/IP, TPM, ACPI 지원같은 핵심 드라이버 파일
 - Ntdll.dll : 내부 지원 함수와 익스큐티브 함수에 대한 시스템 서비스 디스패치 스텁
 - Kernel32.dll, Advapi32.dll, User32.dll, Gdi32.dll : 핵심 윈도우 서비스시템 Dll들
## 2-3 VBS(Virtualization-Based Security) 아키텍처
 
VTL(Virtual Trust Level) 개념 도입 -> VTL 0 = 일반 커널, VTL 1 = 보안 커널(Secure Kernel)
 
VTL은 권한이 아닌 격리를 제공 -> VTL 0 커널조차 VTL 1 메모리에 접근 불가
 
IUM(Isolated User Mode) : VTL 1에서 실행되는 사용자 모드 환경
 
Trustlet : IUM에서 실행 가능한 특수 서명된 바이너리 ( LsaIso.exe, BioIso.exe 등 )
 
SLAT(Second Level Address Translation) : Credential Guard, Device Guard 구현 수단
 
IOMMU : 드라이버가 하이퍼바이저 물리 메모리에 DMA로 접근하는 것을 차단
 
---
 
# 3. 프로세스와 잡
 
## 3-1 프로세스 생성
 
### 프로세스 생성 API 계층
 
```
CreateProcess / CreateProcessAsUser / CreateProcessWithTokenW / CreateProcessWithLogonW
                    ↓
            CreateProcessInternal
                    ↓
            NtCreateUserProcess (유저모드 → 커널모드 전환)
```
 
ShellExecute / ShellExecuteEx -> 파일 확장자 기반으로 실행 파일을 찾아 CreateProcess 호출
 
### CreateProcess 주요 인자
 
- CREATE_SUSPENDED : 초기 스레드를 일시 중단 상태로 생성
- DEBUG_PROCESS : 생성자가 디버거임을 선언
- STARTUPINFO / STARTUPINFOEX : 프로세스 생성 추가 설정
- PROCESS_INFORMATION : 결과로 새 프로세스 ID, 스레드 ID, 핸들 반환
 
### CreateProcess 7단계 흐름
 
1. 파라미터 변환 및 유효성 검사 (우선순위 클래스 결정, 속성 목록 변환 등)
2. 실행할 이미지 파일 오픈 (Image File Execution Options 레지스트리 키 확인)
3. 윈도우 익스큐티브 프로세스 객체 생성
4. 초기 스레드 생성 (스택, 컨텍스트, 스레드 객체)
5. 윈도우 서브시스템(Csrss)에 프로세스 초기화 알림
6. 초기 스레드 실행 시작 (CREATE_SUSPENDED가 아닌 경우)
7. 새 프로세스 컨텍스트에서 주소 공간 초기화 및 DLL 로드 → 엔트리 포인트 실행
 
## 3-2 프로세스 내부 구조
 
### EPROCESS (Executive Process)
 
프로세스를 표현하는 핵심 커널 데이터 구조
 
첫 번째 멤버 = PCB (Process Control Block) -> KPROCESS 구조체
 
```
EPROCESS
├── PCB (KPROCESS) : 스케줄링·시간 계산 데이터
├── UniqueProcessId : PID
├── ActiveProcessLinks : 전체 프로세스 링크드 리스트
├── Win32Process : W32PROCESS 포인터 (GUI 프로세스용)
├── Job : EJOB 포인터 (잡 소속 시)
├── ObjectTable : 핸들 테이블
├── DebugPort : 디버거 포트
└── WoW64Process : 32비트 프로세스 정보 (64비트 OS에서)
```
 
### PEB (Process Environment Block)
 
유저 주소 공간에 존재하는 유일한 프로세스 데이터 구조
 
이미지 로더, DLL 목록, 환경 변수, 힙 정보 등 유저모드 컴포넌트가 접근하는 정보 포함
 
### 병렬 구조
 
- CSR_PROCESS : 윈도우 서브시스템(Csrss)이 관리
- W32PROCESS : Win32k.sys(GUI 커널 드라이버)가 관리
- DXGPROCESS : DirectX 그래픽 커널(Dxgkrnl.sys)이 관리
 
## 3-3 보호 프로세스 (Protected Process)
 
### 배경
 
디지털 저작권 관리(DRM)를 위해 Vista/Server 2008에서 도입
 
관리자 권한으로도 일부 접근 권한이 차단됨
 
### 허용되는 접근 권한만
 
PROCESS_QUERY/SET_LIMITED_INFORMATION, PROCESS_TERMINATE, PROCESS_SUSPEND_RESUME
 
### 보호 프로세스 예시
 
- Audiodg.exe : 보호 음악 콘텐츠 디코딩
- Mfpmp.exe : Media Foundation Protected Pipeline
- Werfaultsecure.exe : WER 클라이언트
- System 프로세스 : 커널 핸들 테이블 무결성 보호
 
## 3-4 PPL (Protected Process Light)
 
Windows 8.1 / Server 2012 R2에서 도입된 확장 보호 모델
 
Signer 값에 따라 보호 수준이 결정됨 (높은 값일수록 더 강한 보호)
 
| Signer | 수준 | 사용 예 |
|--------|------|---------|
| WinSystem | 최고 | System 프로세스, Memory Compression |
| WinTcb | 높음 | smss.exe, csrss.exe, services.exe, wininit.exe |
| Windows | 중간 | sppsvc.exe (소프트웨어 보호 플랫폼) |
| Antimalware | 중간 | MsMpEng.exe (Windows Defender) |
 
### 서드파티 PPL 지원
 
ELAM(Early-Launch Anti-Malware) 드라이버 필요 -> Microsoft에서 발급하는 특수 안티멀웨어 인증서 필요
 
안티멀웨어 소프트웨어가 PPL로 실행 -> 코드 인젝션, 프로세스 강제 종료 불가
 
## 3-5 미니멀 프로세스와 피코 프로세스
 
### 미니멀 프로세스 (Minimal Process)
 
NtCreateProcessEx에 특수 플래그 + 커널 모드 호출자 조합으로 생성
 
특징 :
- 유저모드 주소 공간 미설정 → PEB 없음
- Ntdll.dll 미매핑
- 실행 이미지 파일 미연결
 
예시 : System 프로세스, Memory Compression 프로세스, Secure System 프로세스
 
### 피코 프로세스 (Pico Process)
 
Pico 프로바이더(커널 드라이버)가 미니멀 프로세스의 모든 실행을 제어
 
프로바이더는 PsRegisterPicoProvider API로 등록 (부팅 초기에 단 하나만 등록 가능)
 
프로바이더가 받는 콜백 : 시스템 콜, 예외, 페이지 폴트, 프로세스 이름 조회, 핸들 오픈, 종료 등
 
WSL(Windows Subsystem for Linux)이 피코 프로세스 기반 → Lxss.sys + LxCore.sys가 Pico 프로바이더 역할
 
## 3-6 트러스틀릿 (Trustlet, 보안 프로세스)
 
VTL 1 IUM에서 실행되는 특수 서명된 PE 실행파일
 
제한 사항 :
- 제한된 DLL만 임포트 가능 (C/C++ 런타임, KernelBase, Advapi, RPC, NTDLL 등)
- IUM 전용 DLL(Iumbase.dll) 추가 사용 가능
- .tPolicy PE 섹션에 정책 메타데이터 포함 필수
- IUM EKU(1.3.6.1.4.311.10.3.37) 인증서로 서명 필수
 
IUM 전용 시스템 콜 : 보안 디바이스, 보안 섹션, 메일박스, IDK(ID Key), 암호화 서비스, 보안 스토리지
 
윈도우 10 내장 트러스틀릿 :
- ID 1 : LsaIso.exe (Credential Guard)
- ID 2 : Vmsp.exe (vTPM)
- ID 3 : ? (Reserved)
- ID 4 : BioIso.exe (Secure Biometrics)
- ID 5 : CredentialEnclaveHost.exe (Key Guard)
 
## 3-7 이미지 로더
 
Ntdll.dll 안에 존재하는 유저모드 코드 (내부 명칭 : Ldr)
 
모든 프로세스에 항상 로드됨 → 새 프로세스에서 유저모드 코드 중 가장 먼저 실행
 
### 주요 역할
 
- 초기 힙 생성, TLS/FLS 슬롯 설정
- 임포트 테이블(IAT) 파싱 → 필요한 DLL 재귀적 로드
- 런타임 DLL 로드/언로드 및 모듈 데이터베이스 관리
- 매니페스트 파일(SxS), MUI 파일 처리
- 애플리케이션 호환성 데이터베이스 조회 및 shim 엔진 초기화
- API Set 및 API 리다이렉션 처리 (OneCore/UWP 지원)
- SwitchBack 메커니즘을 통한 동적 호환성 마이그레이션
 
## 3-8 잡 (Job)
 
이름 지정, 보안, 공유 가능한 커널 객체 -> 하나 이상의 프로세스를 그룹으로 제어
 
CreateJobObject로 생성, AssignProcessToJobObject로 프로세스 추가
 
### 주요 한도 설정 (SetInformationJobObject)
 
- 최대 활성 프로세스 수
- 잡 전체 CPU 시간 한도
- 프로세스별 CPU 시간 한도
- 프로세서 선호도(affinity) 설정
- 프로세스 우선순위 클래스
- 가상 메모리 커밋 한도
- CPU 사용률 제어 (스로틀링)
- 네트워크 대역폭 제어
- 디스크 I/O 대역폭 제어
 
### 중첩 잡 (Nested Jobs)
 
Windows 8 / Server 2012부터 프로세스가 여러 잡에 동시 소속 가능
 
자식 잡의 한도는 부모 잡보다 더 제한적이어야 함 (더 관대하게 설정 불가)
 
### 잡의 활용 예
 
- UWP 앱 관리 (모든 모던 앱은 잡 안에서 실행)
- 윈도우 컨테이너(서버 사일로) 구현
- DAM(Desktop Activity Moderator)의 스로틀링
- DFSS(Dynamic Fair Share Scheduling) 스케줄링 그룹
- Chrome, Office 등의 보안 샌드박스
 
## 3-9 윈도우 컨테이너 (서버 사일로)
 
Docker 지원을 위한 경량 OS 격리 기술
 
잡 객체의 확장(super-job) 형태로 구현 -> 이를 하이브리드 잡(hybrid job)이라 부름
 
### 격리 구성 요소
 
- 기본 OS 이미지(WIM) : Server Core 또는 Nano Server
- 샌드박스 가상 파일 시스템 : Wcifs.sys 필터 드라이버
- 샌드박스 가상 레지스트리 : VReg 커널 컴포넌트
- 커스텀 오브젝트 관리자 루트 네임스페이스 (\)
- 사일로별 공유 사용자 데이터 (SILO_USER_SHARED_DATA)
- 사일로별 ETW 트레이싱 컨텍스트
 
호스트 OS 자체도 루트 호스트 사일로(PspHostSiloGlobals)로 취급
 
---
 
# 4. 스레드
 
## 4-1 스레드 생성 API
 
- CreateThread : 현재 프로세스에 스레드 생성
- CreateRemoteThread : 다른 프로세스에 스레드 주입 (디버거에서 활용)
- CreateRemoteThreadEx : 위 둘의 상위 집합, 속성 목록 지원
- PsCreateSystemThread (WDK) : 커널 모드 시스템 스레드 생성
 
최종적으로 모두 NtCreateThreadEx → 커널의 NtCreateThreadEx 함수로 전달
 
## 4-2 스레드 내부 구조
 
### ETHREAD (Executive Thread)
 
스레드를 표현하는 핵심 커널 데이터 구조
 
첫 번째 멤버 = Tcb (Thread Control Block) -> KTHREAD 구조체
 
```
ETHREAD
├── Tcb (KTHREAD) : 스케줄링·동기화·시간 계산
├── CreateTime / ExitTime
├── UniqueThread (CID 내 TID)
├── Cid (CLIENT_ID) : 프로세스 ID + 스레드 ID
├── SecurityContext : 액세스 토큰 + 가장(impersonation) 정보
└── Silo : 소속 잡(서버 사일로용)
```
 
### TEB (Thread Environment Block)
 
유저 주소 공간에 존재 -> 이미지 로더와 각종 DLL이 사용
 
TIB(Thread Information Block)를 헤더로 포함 -> OS/2·Win9x 호환성
 
스택 정보, TLS 슬롯, 마지막 에러 코드, 현재 로케일 등 보관
 
### 보호 프로세스와 스레드
 
보호 프로세스 내 스레드도 제한된 접근 권한만 허용
 
Process Explorer는 유저모드 API로 정보 조회 -> 보호 프로세스의 DLL 목록 확인 불가
 
## 4-3 우선순위 레벨
 
Windows는 0~31 사이의 32개 우선순위 레벨 사용
 
- 0~15 : 변동(dynamic) 우선순위 범위 (0은 Zero Page 스레드 전용)
- 16~31 : 실시간(real-time) 우선순위 범위
 
### API 관점의 우선순위
 
프로세스 우선순위 클래스 (SetPriorityClass) :
 
| 클래스 | 내부 번호 | 기본 우선순위 |
|--------|----------|--------------|
| Idle | 1 | 4 |
| Below Normal | 5 | 6 |
| Normal | 2 | 8 |
| Above Normal | 6 | 10 |
| High | 3 | 13 |
| Real-time | 4 | 24 |
 
스레드 상대 우선순위 (SetThreadPriority) :
 
| 레벨 | 델타 |
|------|------|
| Time-Critical | +15 (포화값) |
| Highest | +2 |
| Above-Normal | +1 |
| Normal | 0 |
| Below-Normal | -1 |
| Lowest | -2 |
| Idle | -15 (포화값) |
 
### 동적 vs 기본 우선순위
 
각 스레드는 기본(base) 우선순위와 현재(dynamic) 우선순위 두 가지를 가짐
 
스케줄러는 현재 우선순위 기반으로 결정
 
실시간 범위(16~31)는 OS가 우선순위를 조정하지 않음
 
## 4-4 스레드 상태
 
| 상태 | 설명 |
|------|------|
| 초기화(Initialized) | 스레드 생성 중 내부 상태 |
| 준비(Ready) | 실행 대기 중 |
| 지연 준비(Deferred Ready) | 특정 프로세서에서 실행 예약됨(락 최소화용) |
| 대기(Standby) | 다음에 실행될 스레드로 선택됨 |
| 실행(Running) | 현재 실행 중 |
| 대기(Waiting) | 커널 객체 대기 중 또는 I/O 완료 대기 |
| 전환(Transition) | 실행 준비됐지만 커널 스택이 페이지 아웃된 상태 |
| 종료(Terminated) | 실행 완료 |
 
## 4-5 디스패처 데이터베이스
 
스케줄러가 어떤 스레드를 실행할지 결정하는 데이터 구조 집합
 
Windows 8 / Server 2012부터 프로세서 그룹 단위 공유 준비 큐(KSHARED_READY_QUEUE) 사용
 
32개 우선순위별 준비 큐(ReadListHead) + 32비트 준비 요약(ReadySummary) 비트마스크
 
ReadySummary에서 최상위 비트 스캔(native 명령) → 일정한 시간에 다음 스레드 결정
 
## 4-6 퀀텀 (Quantum)
 
스레드가 한 번에 실행될 수 있는 시간 단위
 
- 클라이언트 Windows : 기본 2 클록 인터벌 (쿼텀 값 6)
- 서버 Windows : 기본 12 클록 인터벌 (쿼텀 값 36)
 
클록 인터벌 : x86/x64 멀티프로세서에서 약 15ms
 
내부적으로 퀀텀 = 클록 틱의 1/3 단위로 저장
 
퀀텀 만료는 CPU 클록 사이클 기준으로 측정 (KeMaximumIncrement, KiCyclesPerClockQuantum)
 
### 가변 퀀텀
 
포그라운드 프로세스의 스레드에 퀀텀 3배 부여 (클라이언트 기본값)
 
HKLM\SYSTEM\CurrentControlSet\Control\PriorityControl\Win32PrioritySeparation 레지스트리 값으로 제어
 
## 4-7 우선순위 부스트
 
시스템이 일시적으로 스레드 우선순위를 동적 범위(1~15)에서 올려주는 메커니즘
 
실시간 범위(16~31)에는 부스트 적용 안 됨
 
주요 부스트 상황 :
- 대기 완료 후 포그라운드 스레드
- GUI 스레드가 깨어날 때
- I/O 완료 후
- 멀티미디어 애플리케이션
 
스타베이션(starvation) 방지 : 약 4초마다 기아 상태 스레드에 일시 부스트
 
## 4-8 CPU 집합과 프로세서 선택
 
NUMA(Non-Uniform Memory Access) 시스템에서 노드 친화도 기반 스케줄링
 
이상 프로세서(Ideal Processor) : 각 스레드의 선호 CPU
 
친화도 마스크(Affinity Mask) : 스레드가 실행될 수 있는 CPU 비트마스크
 
프로세서 그룹 : 최대 64 논리 프로세서 단위 (Windows는 최대 640개 논리 프로세서 지원)
 
SMT(Simultaneous Multithreading) 인식 : 같은 물리 코어의 논리 프로세서보다 다른 물리 코어를 우선 선택
 
---
 
# 5. 메모리 관리
 
## 5-1 메모리 관리자 개요
 
Ntoskrnl.exe의 가장 큰 컴포넌트 (HAL에는 메모리 관리자 없음)
 
### 주요 역할
 
1. 가상 주소 → 물리 주소 변환 (워킹셋 = 현재 물리 메모리에 상주하는 가상 페이지 집합)
2. 메모리 초과 커밋 시 디스크로 페이징 → 필요 시 물리 메모리로 복귀
 
### 가상 주소 공간 크기
 
| OS | 프로세스 가상 공간 |
|----|-------------------|
| 32비트 Windows | 2GB (특수 플래그 시 최대 3GB/4GB) |
| 64비트 Windows 8 / Server 2012 | 8TB |
| 64비트 Windows 8.1 이상 | 128TB |
 
### 메모리 관리자 내부 스레드 (모두 System 프로세스 소속)
 
- Balance Set Manager (우선순위 17) : 초당 1회 워킹셋 관리자 호출
- Process/Stack Swapper (우선순위 23) : 프로세스·스택 스왑
- Modified Page Writer (우선순위 18) : 변경 페이지를 페이징 파일에 기록
- Mapped Page Writer (우선순위 18) : 매핑된 파일의 변경 페이지를 디스크에 기록
- Segment Dereference Thread (우선순위 19) : 캐시 축소, 페이징 파일 증감
- Zero Page Thread (우선순위 0) : 빈 페이지를 0으로 초기화
 
## 5-2 페이지
 
메모리 관리 최소 단위, 하드웨어 MMU가 가상→물리 변환하는 단위
 
| 아키텍처 | 소형 페이지 | 대형 페이지 | 초대형 페이지 |
|----------|------------|------------|--------------|
| x86 | 4KB | 4MB | - |
| x64 | 4KB | 2MB | 1GB |
| ARM | 4KB | 4MB | - |
 
### 대형 페이지 장점
 
TLB(Translation Lookaside Buffer) 엔트리 절약 → 주소 변환 속도 향상
 
OS 핵심 이미지(Ntoskrnl.exe, Hal.dll)와 초기 논페이지 풀은 대형 페이지로 매핑
 
### 주의사항
 
대형 페이지는 단일 보호 속성만 가능 → 읽기 전용 코드와 읽기/쓰기 데이터를 같은 페이지에 배치 불가
 
대형 페이지는 항상 논페이지 상태 → SeLockMemoryPrivilege 권한 필요
 
## 5-3 메모리 관리자가 제공하는 API 그룹
 
- Virtual API : 페이지 단위의 저수준 메모리 할당 (VirtualAlloc, VirtualFree, VirtualProtect, VirtualLock)
- Heap API : 소규모 할당용 (HeapAlloc, HeapFree, HeapCreate), Virtual API를 내부적으로 사용
- Local/Global API : 16비트 Windows 호환 레거시, 현재 Heap API로 구현
- Memory-Mapped Files : 파일을 메모리로 매핑 (CreateFileMapping, MapViewOfFile)
 
C/C++ 런타임의 malloc/free/new/delete는 Heap API를 사용
 
## 5-4 페이지 상태
 
| 상태 | 설명 |
|------|------|
| Free | 미예약·미커밋 상태, 접근 시 접근 위반 예외 |
| Reserved | 주소 공간 예약됨(물리 메모리 미사용), 접근 시 접근 위반 예외 |
| Committed | 실제 물리/페이지 파일 저장소에 매핑됨, 처음 접근 시 zero-filled |
| Shareable | 여러 프로세스가 공유 가능한 섹션에 매핑됨 |
 
### 예약(Reserve) vs 커밋(Commit)
 
예약 : 가상 주소 공간 점유, 물리 자원 미소비
 
커밋 : 실제 저장소(물리 메모리 또는 페이징 파일) 할당
 
스레드 스택 : 기본 1MB 예약, 초기 1페이지만 커밋 + 가드 페이지 → 스택 자동 확장
 
### 커밋 한도
 
시스템 전체 커밋된 가상 메모리 총량 = 커밋 차지(Commit Charge)
 
시스템 커밋 한도 = 전체 페이징 파일 크기 + 사용 가능한 RAM
 
메모리 관리자가 필요 시 페이징 파일을 자동으로 확장하여 한도 늘림
 
## 5-5 공유 메모리와 매핑 파일
 
섹션 객체(Section Object) = Windows API의 파일 매핑 객체(File-Mapping Object)
 
### 동작 원리
 
DLL 코드 페이지는 한 번만 물리 메모리에 로드 → 여러 프로세스가 공유
 
개인 데이터는 COW(Copy-on-Write)를 통해 쓰기 시점에 분리
 
### 섹션 생성
 
```
CreateFileMapping(파일 핸들 또는 INVALID_HANDLE_VALUE)
    → 파일 매핑 오브젝트 생성
MapViewOfFile(섹션 핸들, 오프셋, 크기)
    → 프로세스 주소 공간에 뷰 매핑
```
 
INVALID_HANDLE_VALUE → 페이지 파일 기반 공유 메모리 (zero-initialized로 초기화)
 
### 활용
 
- 이미지 로더 : EXE, DLL 로드 시 섹션 객체 사용
- 캐시 관리자 : 파일 캐시에 섹션 오브젝트 사용
- IPC : 프로세스 간 공유 메모리
 
## 5-6 메모리 보호
 
### 4가지 보호 방법
 
1. 커널모드 전용 페이지 : 유저모드 접근 시 하드웨어 폴트 → 접근 위반
2. 개인 가상 주소 공간 : 각 프로세스는 다른 프로세스 메모리에 직접 접근 불가
3. 공유 메모리 섹션 : ACL(접근 제어 목록)로 보호
4. COW(Copy-on-Write) : 공유 페이지를 수정 시 개인 복사본 생성
 
### 페이지 보호 속성
 
PAGE_READONLY, PAGE_READWRITE, PAGE_EXECUTE, PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE, PAGE_NOACCESS, PAGE_GUARD, PAGE_NOCACHE 등
 
### DEP (Data Execution Prevention)
 
데이터 페이지를 실행 불가(non-executable)로 표시하는 하드웨어 기반 보호
 
CPU의 NX(No-Execute) / XD(Execute Disable) 비트 활용
 
## 5-7 주소 변환
 
### x86 페이지 테이블 (4단계는 x64)
 
PML4 → PDPT → PD → PT → 물리 프레임
 
TLB(Translation Lookaside Buffer) : 최근 가상→물리 변환 캐시
 
TLB 미스 → 페이지 테이블 워크(page table walk)
 
### ASLR (Address Space Layout Randomization)
 
이미지, 힙, 스택의 주소를 무작위화하여 공격자가 특정 주소를 예측하기 어렵게 함
 
커널 주소 공간도 ASLR 적용 (드라이버 64개 로드 주소, 힙/풀/PFN 데이터베이스 등)
 
## 5-8 페이지 폴트 처리
 
접근한 가상 주소가 물리 메모리에 없을 때 하드웨어가 폴트 생성 → 메모리 관리자가 처리
 
### 폴트 유형
 
- 수요 제로 폴트(Demand-Zero Fault) : 커밋됐지만 아직 미사용 페이지 → 0 초기화 후 제공
- 전환 폴트(Transition Fault) : 페이지가 대기 목록에 있음 → 물리 메모리에서 재활성화
- 페이징 파일 읽기(Paging File Read) : 페이지 파일에서 디스크 I/O
- 매핑 파일 읽기(Mapped File Read) : 파일에서 디스크 I/O
 
## 5-9 워킹셋 (Working Set)
 
프로세스가 현재 물리 메모리에 보유 중인 페이지 집합
 
### 워킹셋 크기
 
- 최소 워킹셋 : 프로세스가 보장받는 최소 물리 페이지 수
- 최대 워킹셋 : 프로세스가 가질 수 있는 최대 물리 페이지 수
- SetProcessWorkingSetSizeEx로 조절 가능
 
### 워킹셋 트리밍
 
Balance Set Manager가 메모리 부족 시 프로세스 워킹셋 축소
 
트리밍된 페이지는 대기 목록(Standby List) 또는 변경 목록(Modified List)으로 이동
 
## 5-10 페이지 목록
 
| 목록 | 설명 |
|------|------|
| 제로 목록(Zeroed) | 0으로 초기화된 빈 페이지 |
| 빈 목록(Free) | 초기화되지 않은 빈 페이지 |
| 대기 목록(Standby) | 워킹셋에서 제거됐지만 아직 디스크에 기록 안 됨(소프트 폴트로 재사용 가능) |
| 변경 목록(Modified) | 수정됐으나 디스크에 기록 안 됨 |
| 변경 없음 목록(Modified No Write) | 수정됐으나 기록 금지 |
| 불량 목록(Bad) | 하드웨어 오류가 발생한 페이지 |
 
## 5-11 물리 메모리 한도
 
Windows 버전에 따라 지원하는 최대 물리 메모리 상이
 
64비트 Windows 10 Pro : 128GB
 
64비트 Windows 10 Enterprise/Education : 2TB
 
64비트 Windows Server 2019 Standard : 24TB
 
## 5-12 메모리 파티션 (Memory Partitions)
 
Windows 10 / Server 2016에서 도입
 
각 파티션은 독립적인 페이지 목록(제로/빈/대기/변경)과 PFN 데이터베이스 범위를 가짐
 
잡(Job)을 통해 프로세스에 파티션 지정 가능
 
Hyper-V, Windows 컨테이너에서 활용
 
## 5-13 메모리 결합 (Memory Combining)
 
동일한 내용의 물리 페이지를 찾아 하나로 합병하고 COW로 공유
 
기본적으로 활성화, 주기적으로 실행
 
합병 가능한 페이지 : 개인 커밋 페이지 중 페이지 파일로 백업된 것
 
## 5-14 메모리 엔클레이브 (Memory Enclaves)
 
VBS 기반의 신뢰 실행 환경 (TEE, Trusted Execution Environment)
 
VBS-based Enclave : 하이퍼바이저 기반 메모리 보호
 
Intel SGX Enclave : 하드웨어 기반 엔클레이브 (프로세서 지원 필요)
 
InitializeEnclave / CreateEnclave API로 생성
 
## 5-15 슈퍼패치 / 프리패처 (SuperFetch / Prefetcher)
 
### 프리패처 (Prefetcher)
 
애플리케이션 시작 시 필요한 파일·데이터를 미리 읽어 성능 향상
 
%SystemRoot%\Prefetch 폴더에 .pf 확장자 파일로 추적 데이터 저장
 
### 슈퍼패치 (SuperFetch)
 
메모리 사용 패턴을 장기적으로 학습하여 유휴 시간에 자주 쓰는 데이터를 미리 메모리에 적재
 
Sysmain 서비스(SysMain.dll)로 구현
 
구성 요소 :
- 트레이싱 : 메모리 접근 패턴 연속 추적
- 데이터베이스 : 패턴 분석 및 통합 (SysmainDB)
- 최적화 엔진 : 유휴 시간에 예측 프리패칭 수행
 
페이지 우선순위(0~7) 기반으로 대기 목록 내 페이지 관리 → 중요 페이지가 오래 유지됨

---
 
# 6. I/O 시스템
 
## 6-1 I/O 시스템 설계 목표
 
I/O 시스템의 핵심 목표는 물리, 논리, 가상 디바이스를 **일관된 방식으로 추상화**하는 것
 
- 디바이스 전반에 걸친 균일한 보안과 명명 체계
- 고성능 비동기 패킷 기반 I/O (스케일러블 서버 앱 구현용)
- 드라이버를 C/C++ 고급 언어로 작성하고 다른 아키텍처로 이식 가능하게
- 계층화·확장성 → 기존 드라이버를 수정하지 않고 새 드라이버 삽입 가능
- 드라이버 동적 로드, 언로드
- PnP 지원 → 드라이버 자동 탐지, 설치, 자원 할당
- 전원 관리 지원
- 다중 설치형 파일 시스템 지원 (FAT, NTFS, CDFS, UDF, ReFS 등)
- WMI를 통한 드라이버 관리 및 모니터링
## 6-2 I/O 시스템 구성 요소
 
```
유저 모드
 └── 일반 애플리케이션 → CreateFile / ReadFile / WriteFile
커널 모드
 ├── I/O 관리자   ← IRP 생성, 전달, 처리 (핵심)
 ├── PnP 관리자  ← 장치 감지, 드라이버 로드, 자원 할당
 ├── 전원 관리자  ← 전원 상태 전환 정책 결정
 ├── 디바이스 드라이버 스택
 └── HAL
```
 
모든 I/O 요청은 가상 파일(virtual file)에 대한 연산으로 추상화된다
 
`CreateFile`에서 C: 같은 이름은 오브젝트 관리자의 `GLOBAL??` 디렉터리의 심볼릭 링크 → 내부적으로 `\Device\HarddiskVolume7` 같은 이름에 연결
 
드라이버는 Open, Close, Read, Write 같은 파일 지향 명령을 하드웨어 전용 명령으로 변환하는 역할을 한다
 
## 6-3 IRQL (Interrupt Request Level)
 
CPU별로 가지는 인터럽트 우선순위 레벨
 
**기본 규칙** : 높은 IRQL 코드가 낮은 IRQL 코드를 선점한다. 반대는 불가
 
| IRQL | 이름 | 실행 주체 |
|------|------|---------|
| 0 | PASSIVE_LEVEL | 일반 유저, 커널 코드 → 스케줄러 정상 동작 |
| 1 | APC_LEVEL | APC 처리 |
| 2 | DISPATCH_LEVEL | 스케줄러, DPC 처리. 이 이상이면 스케줄러 작동 불가 |
| 3-26 (x86) / 3-12 (x64, ARM) | DIRQL | 하드웨어 인터럽트 처리 (ISR) |
 
IRQL 2 이상에서의 제약 :
 
- 커널 디스패처 오브젝트(뮤텍스, 이벤트 등) 대기 불가 → 시스템 크래시
- 페이지 폴트 처리 불가 → 논페이지드 메모리만 접근 가능
유저 모드는 항상 IRQL 0이므로 유저 모드 문서에는 IRQL 개념이 등장하지 않는다
 
## 6-4 DPC (Deferred Procedure Call)
 
하드웨어 인터럽트의 후속 작업을 IRQL 2에서 지연 실행하는 메커니즘
 
ISR → DPC 처리 흐름 :
 
```
하드웨어 인터럽트 발생
    → CPU 상태 저장, IRQL = DIRQL로 상승
ISR 실행 (최소 작업 — 장치 상태 읽기, 인터럽트 신호 해제)
    → KeInsertQueueDpc()로 DPC 큐에 삽입 후 ISR 반환
보류 인터럽트 없으면 IRQL = 2로 하강
DPC 처리 루프 — 순차 실행
    → DPC 큐 비면 IRQL = 0 복구, 원래 코드 재개
```
 
DPC를 쓰는 이유 : DIRQL에서 계속 실행하면 그 이하 IRQL의 인터럽트가 모두 차단됨. 대부분의 처리를 IRQL 2로 내려 다른 인터럽트 서비스 지연을 최소화
 
ISR과 DPC는 어떤 스레드 컨텍스트에서도 실행될 수 있음 → 특정 프로세스의 유저 모드 주소 공간에 의존하는 코드 작성 불가
 
## 6-5 디바이스 드라이버 종류
 
### 유저 모드 드라이버
 
- Windows 서브시스템 프린터 드라이버 : 그래픽 요청 → 프린터 전용 명령 변환
- UMDF 드라이버 : 유저 모드에서 실행하는 하드웨어 드라이버. ALPC로 커널 측과 통신
### 커널 모드 드라이버
 
| 유형 | 설명 |
|------|------|
| 파일 시스템 드라이버 | 파일 I/O 요청 → 스토리지·네트워크 드라이버 요청으로 변환 |
| PnP 드라이버 | 하드웨어 직접 통신. 전원 관리자·PnP 관리자와 통합 |
| 비PnP 드라이버 | 커널 확장 모듈. 실제 하드웨어 없음 (ex. Process Monitor 드라이버) |
 
### WDM 드라이버 세 가지 역할
 
| 역할 | 설명 | 생성 오브젝트 |
|------|------|-------------|
| 버스 드라이버 | 버스(PCI, USB, IEEE 1394 등) 관리. 연결된 장치 감지 → PnP 관리자에 보고 | PDO |
| 기능 드라이버 | 장치의 실제 기능 구현. 장치를 가장 잘 알고 있음 | FDO |
| 필터 드라이버 | 기능 드라이버 위(Upper) 또는 버스 드라이버 위(Lower)에 레이어링 | FiDO |
 
### 계층화 드라이버 추가 분류
 
- 클래스 드라이버 : 디스크·키보드·CD-ROM 등 표준화된 장치 클래스 공통 처리
- 미니클래스 드라이버 : 제조사별 커스텀 구현. 클래스 드라이버 함수를 임포트하는 커널 DLL
- 포트 드라이버 : SATA 같은 특정 I/O 포트 처리 라이브러리. 주로 Microsoft 작성
- 미니포트 드라이버 : 특정 어댑터를 포트 드라이버에 연결. 제조사 작성. NDIS가 네트워크 포트 드라이버
## 6-6 드라이버 구조
 
드라이버는 I/O 관리자가 호출하는 루틴 집합으로 구성된다
 
| 루틴 | 호출 시점 | IRQL |
|------|---------|------|
| DriverEntry (초기화) | 드라이버 로드 시 | 0 |
| AddDevice | PnP 관리자가 장치 감지 시 | 0 |
| 디스패치 루틴 | I/O 요청 도착 시 (IRP_MJ_READ 등) | 0 |
| StartI/O | I/O 전송 시작 | 0 또는 임의 |
| ISR | 하드웨어 인터럽트 발생 시 | DIRQL |
| DPC 루틴 | ISR 이후 지연 처리 | 2 |
| I/O 완료 루틴 | 하위 드라이버 IRP 완료 시 | 0~2 |
| 취소 루틴 | I/O 취소 요청 시 | 2 |
| Fast Dispatch 루틴 | 캐시 관리자 등이 IRP 우회할 때 | 0 |
| 언로드 루틴 | 드라이버 제거 시 | 0 |
 
### 드라이버 오브젝트 (DRIVER_OBJECT)
 
드라이버 로드 시 I/O 관리자가 생성. 각 디스패치 루틴 주소를 저장
 
### 디바이스 오브젝트 (DEVICE_OBJECT)
 
드라이버가 `IoCreateDevice`로 생성. 물리·논리 장치를 표현. 모든 I/O 요청의 실제 대상
 
## 6-7 I/O 요청 패킷 (IRP)
 
I/O 요청을 표현하는 핵심 데이터 구조. I/O 시스템이 패킷 기반으로 동작하는 핵심 단위
 
```
IRP
├── 주 함수 코드 (IRP_MJ_READ, IRP_MJ_WRITE, IRP_MJ_DEVICE_CONTROL 등)
├── 상태 정보
├── MDL 포인터 (물리 메모리 버퍼 서술)
└── I/O 스택 위치 배열 (드라이버마다 하나씩)
    └── [스택 위치 N] : minor 코드, 파라미터, 완료 루틴
```
 
프로세서별 룩어사이드 리스트에서 할당 (소형 1개, 중형 4개, 대형 14개 스택 위치)
 
Fast I/O만 IRP를 사용하지 않는 예외이다
 
## 6-8 I/O 유형
 
| 유형 | 설명 |
|------|------|
| 동기 I/O | 완료될 때까지 호출 스레드 차단 (기본 ReadFile, WriteFile) |
| 비동기 I/O | 즉시 반환. FILE_FLAG_OVERLAPPED 필요. 완료 시 이벤트, APC, 완료 포트로 알림 |
| Fast I/O | IRP 생성 없이 드라이버 스택을 직접 호출. 파일 시스템 드라이버의 캐시 히트 경로에 활용 |
| 매핑 파일 I/O | 파일을 가상 메모리처럼 접근. 메모리 관리자와 협력. 캐시 관리자, 이미지 로더가 활용 |
| 스캐터/개더 I/O | ReadFileScatter / WriteFileGather. 단일 요청으로 여러 분산 버퍼 처리. 비캐시, 비동기, 정렬 필요 |
 
## 6-9 단일 계층 드라이버의 I/O 처리 흐름
 
요청 스레드 컨텍스트 : 원래 I/O를 요청한 스레드가 실행 중 → 유저 버퍼 주소 직접 접근 가능
 
임의 스레드 컨텍스트 : 어떤 스레드가 실행 중인지 모름 → 유저 버퍼 직접 접근 불가, MDL·버퍼드 I/O 방식 필요
 
처리 순서 :
 
```
[요청 스레드 - 유저 모드]
1. ReadFile() 호출
 
[요청 스레드 - 커널 모드]
2. I/O 관리자 : IRP 생성, 드라이버 디스패치 루틴 호출 (IoCallDriver)
3. 디스패치 루틴 : 파라미터 검증, IoStartPacket으로 StartI/O 호출 또는 IRP 큐 삽입
4. StartI/O 루틴 : HAL을 통해 하드웨어 레지스터 프로그래밍
 
[하드웨어 동작 중 → 완료 인터럽트 발생]
 
[임의 스레드 - DIRQL]
5. CPU 상태 저장, ISR 진입
6. ISR : 장치 상태 저장, 인터럽트 해제, KeInsertQueueDpc
 
[임의 스레드 - IRQL 2]
7. DPC 루틴 :
   ① IoStartNextPacket으로 다음 IRP 처리 시작 (장치 유휴 방지)
   ② IoCompleteRequest로 현재 IRP 완료 → 완료 루틴 → 스레드 대기 해제
 
[요청 스레드 재개]
8. I/O 결과 반환
```
 
## 6-10 I/O 취소
 
드라이버가 취소 루틴(`IoSetCancelRoutine`)을 등록하지 않은 IRP를 무기한 보류하면 프로세스가 종료 불가 상태가 된다 (unkillable process 현상)
 
| 함수 | 설명 |
|------|------|
| CancelIo | 현재 스레드의 특정 파일 핸들 비동기 I/O 취소 |
| CancelIoEx | 같은 프로세스의 특정 파일 핸들 전체 스레드 비동기 I/O 취소 |
| CancelSynchronousIo | 다른 스레드의 동기 I/O 취소 |
 
스레드 종료 시 I/O 관리자가 해당 스레드의 IRP 목록을 순회해 취소 루틴이 등록된 IRP를 모두 취소한다. 취소가 완료될 때까지 스레드·프로세스 오브젝트는 해제되지 않는다
 
## 6-11 I/O 완료 포트
 
고성능 서버 애플리케이션을 위한 비동기 I/O 통지 메커니즘. `IoCompletion` 익스큐티브 오브젝트 기반
 
**핵심 아이디어** : 동시성 값(Concurrency Value)으로 활성 스레드 수를 OS가 자동 제어 → 불필요한 컨텍스트 스위치 방지
 
동작 원리 :
 
- `CreateIoCompletionPort`로 완료 포트 생성 (내부에 KQUEUE 커널 오브젝트 초기화)
- 파일 핸들을 포트에 연결
- 스레드들이 `GetQueuedCompletionStatus(Ex)`로 완료 패킷 대기 (LIFO 순서로 깨어남)
- 비동기 I/O 완료 시 패킷이 포트 큐에 삽입 → 대기 스레드 하나가 깨어남
- 활성 스레드가 블록되면 대기 스레드가 자동으로 깨어나 동시성 한도 유지
권장 동시성 값 = 논리 프로세서 수
 
`PostQueuedCompletionStatus`로 커스텀 패킷을 직접 삽입 가능 (서버 종료 신호 전달 등)
 
## 6-12 PnP 관리자
 
하드웨어·드라이버·OS가 협력하는 3계층 메커니즘
 
### 핵심 기능
 
- 장치 자동 감지 : 부팅 시, 최대절전 복귀 시, 명시적 요청 시 장치 열거
- 하드웨어 자원 할당 : 자원 중재(Resource Arbitration)로 인터럽트·I/O 메모리·레지스터를 최적 할당
- 드라이버 자동 로드 : 장치 ID 기반으로 적합한 드라이버 탐색·로드. 미설치 시 유저 모드 PnP 관리자에 설치 요청
- 장치 상태 변경 알림 : 앱·드라이버에 장치 추가/제거 알림
- 네트워크 연결 장치 지원 : 버스 드라이버가 네트워크를 버스로 인식해 장치 노드 생성
### 장치 트리와 데브노드
 
PnP 관리자가 내부적으로 관리하는 장치 계층 구조. 노드를 데브노드(devnode)라 부름
 
Root (가상 버스) → HAL → 주 버스(PCI) → USB·ISA·SCSI 버스 → 각 장치
 
### 장치 스택 구조
 
```
상위 필터 드라이버 (FiDO) ← 선택사항
        ↑
기능 드라이버 (FDO) ← 필수, FDO 하나만 존재
        ↑
하위 필터 드라이버 (FiDO) ← 선택사항
        ↑
버스 드라이버 (PDO) ← 필수, 항상 맨 아래
```
 
IRP는 스택 상단에서 하단으로 전달되며 각 드라이버가 자신의 스택 위치에서 파라미터를 읽고 처리한다
 
### PnP 장치 상태 전환 주요 IRP
 
| IRP | 의미 |
|-----|------|
| IRP_MN_START_DEVICE | 장치 시작 |
| IRP_MN_QUERY_STOP_DEVICE | 자원 재배치를 위한 일시 중단 가능 여부 질의 |
| IRP_MN_STOP_DEVICE | 장치 일시 중단 |
| IRP_MN_QUERY_REMOVE_DEVICE | 장치 제거 가능 여부 질의 |
| IRP_MN_REMOVE_DEVICE | 장치 제거 |
| IRP_MN_SURPRISE_REMOVAL | 예고 없는 장치 제거 (사용자가 뽑아버린 경우) |
 
### 드라이버 로드 순서 (Start 값)
 
| 값 | 이름 | 로드 시점 | 예시 |
|---|------|---------|------|
| 0 | boot-start | 부트 로더 | 시스템 버스 드라이버, 부트 파일 시스템 |
| 1 | system-start | I/O 관리자 (Executive 초기화 후) | 시리얼 포트 드라이버 |
| 2 | auto-start | 자동 시작 | MUP (UNC 경로 지원) |
| 3 | demand-start | PnP 장치 감지 시 | 네트워크 어댑터 드라이버 |
 
### 드라이버 설치
 
PnP 드라이버 : INF 파일 필수. 하드웨어 장치 ID, 파일 복사 지침, 레지스트리 수정, 의존성 정보 포함
 
소프트웨어 전용 드라이버 (ex. Process Explorer의 PROCEXP152) : INF 없이 `CreateService` API로 설치 후 `StartService`로 로드 가능
 
## 6-13 Windows Driver Foundation (WDF)
 
오픈소스 드라이버 개발 프레임워크 (https://github.com/Microsoft/Windows-Driver-Frameworks)
 
WDM의 복잡성(PnP·전원·I/O 큐잉·DMA·동기화)을 자동 처리하는 추상화 계층
 
### KMDF (Kernel-Mode Driver Framework)
 
WDM 기반 드라이버의 공통 작업을 자동 처리
 
KMDF 핵심 개념 :
 
- 오브젝트 계층 : WDFDRIVER → WDFDEVICE → WDFQUEUE → WDFREQUEST 등 부모-자식 관계. 부모 삭제 시 자식도 자동 해제
- 컨텍스트 영역 : 모든 WDF 오브젝트에 드라이버 전용 데이터를 붙일 수 있음
- I/O 큐 : Parallel·Sequential·Manual 세 가지 자동 디스패칭. 드라이버가 IRP 대신 WDFREQUEST 래퍼 사용
유니버설 Windows 드라이버 (Windows 10~) : IoT Core, Mobile, 데스크톱 모두에서 바이너리 호환. KMDF, UMDF 2.x, WDM으로 구축 가능
 
### UMDF (User-Mode Driver Framework)
 
드라이버를 유저 모드(WUDFHost.exe 프로세스 내)에서 실행
 
장점 :
 
- 크래시 시 시스템이 아닌 호스트 프로세스만 종료
- Local Service 계정으로 실행 → 공격 표면 최소화
- 항상 IRQL 0 → 페이지 폴트 처리 가능, 디스패처 오브젝트 사용 가능
- 디버깅이 KMDF보다 쉬움 (두 머신 불필요)
단점 : 커널/유저 전환으로 인한 추가 지연. 고속 PCI 장치에는 부적합
 
적합한 대상 : IEEE 1394, USB, Bluetooth, HID, TCP/IP 프로토콜 기반 장치
 
UMDF 아키텍처 구성 요소 :
 
| 구성 요소 | 역할 |
|---------|------|
| 리플렉터 (WUDFRd.sys) | 디바이스 스택 최상단의 WDM 필터 드라이버. IRP를 ALPC로 호스트 프로세스에 전달 |
| 드라이버 관리자 (WUDFsvc.dll) | 호스트 프로세스 시작·종료 관리. 자동 시작 Windows 서비스 |
| 호스트 프로세스 (WUDFHost.exe) | 실제 UMDF 드라이버가 실행되는 주소 공간. 장치 인스턴스마다 별도 프로세스 |
 
## 6-14 전원 관리자
 
ACPI 사양 기반으로 시스템·장치 전원 상태를 관리한다
 
### 시스템 전원 상태 (S 상태)
 
| 상태 | 이름 | 설명 |
|------|------|------|
| S0 | 완전 동작 | CPU 활성, 모든 기능 사용 |
| S1~S3 | 절전 (슬립) | 메모리 내용 유지. S3이 가장 일반적 |
| S4 | 최대절전 | 메모리 내용을 Hiberfil.sys에 저장 후 전원 차단. Winresume.exe가 복원 |
| S5 | 완전 차단 | 소프트웨어 상태 없음. 정상 부팅 필요 |
| S0 Low Power Idle | Modern Standby | S0이지만 CPU 활동 최소화. UWP 백그라운드 작업만 허용. Instant On 가능 |
 
하이브리드 슬립 : S3(메모리 유지) + S4(응급 최대절전 파일 생성) 조합. 전원 차단 시 S4에서 안전하게 복구
 
### 장치 전원 상태 (D 상태)
 
| 상태 | 설명 |
|------|------|
| D0 | 완전 동작 |
| D1, D2 | 장치별 중간 절전 |
| D3-hot | 대부분 꺼짐, 주전원 연결 유지. 버스 컨트롤러가 장치 인식 가능 |
| D3-cold | 주전원 차단 (Windows 8~). 최대 절전 |
 
### 전원 관리자 동작 원리
 
시스템 전원 상태 전환 결정 요소 : 시스템 활동 수준, 배터리 잔량, 앱의 종료·최대절전 요청, 사용자 동작(전원 버튼), 제어판 전원 설정
 
전환 시 `IRP_MJ_POWER`로 드라이버에 전달 → 장치 전원 정책 소유자(FDO)가 적절한 D 상태 결정 → `PoRequestPowerIrp`로 전원 관리자에 요청 → 전원 관리자가 다른 드라이버에 전달
 
드라이버는 전원 전환에 거부권을 행사할 수 없다 (최대절전은 항상 성공). 최대 2초간 상태 정리 허용
 
### PoFx (Power Management Framework, Windows 8~)
 
장치 내부의 개별 컴포넌트별 전원 상태 관리 프레임워크
 
F 상태 : F0(완전 동작) ~ Fn(저전력). D0 상태에서만 의미 있음
 
- `PoFxRegisterDevice` : 장치를 PoFx에 등록
- `PoFxActivateComponent` : 컴포넌트를 F0으로 요청
- `PoFxIdleComponent` : 컴포넌트가 불필요해졌음을 알림
Windows 10에서 성능 상태 관리 추가 : GPU 클록 주파수, 대역폭 등 D0 상태에서의 소비 전력 세밀 제어 (`PoFxRegisterComponentPerfStates`)
 
### 전원 가용성 요청
 
앱·드라이버가 특정 전원 전환을 방지하는 요청. 커널 오브젝트 관리자에 등록된 정식 오브젝트
 
| 요청 유형 | 설명 |
|---------|------|
| System Request | 유휴 타이머로 인한 슬립 방지 |
| Display Request | 화면 꺼짐 방지 |
| Away Mode | 슬립처럼 보이지만 실제로는 S0 유지. 미디어 계속 전달용 |
| Execution Required | UWP 앱 프로세스 계속 실행 요청 (Modern Standby 지원 시스템에서) |
 
`PowerCreateRequest` / `PowerSetRequest` / `PowerClearRequest` (유저 모드)
 
`PoCreatePowerRequest` / `PoSetPowerRequest` / `PoClearPowerRequest` (커널 모드)

---
 
# 7. 보안 (Security)
 
## 7-1 보안 등급
 
Windows 설계에 영향을 준 두 가지 보안 평가 기준이 있다
 
**TCSEC (Trusted Computer System Evaluation Criteria)** : 미국 오렌지북. C2 등급을 목표로 설계됨. 자원 접근 제어, 감사, 사용자 인증, 메모리 재사용 보호 등 요구
 
**CC (Common Criteria)** : TCSEC를 대체하는 현행 국제 보안 평가 표준. Windows는 CAPP(Controlled Access Protection Profile) 하에 CC 인증 획득
 
## 7-2 보안 시스템 구성 요소
 
| 구성 요소 | 위치 | 역할 |
|---------|------|------|
| SRM (Security Reference Monitor) | 커널 (Ntoskrnl.exe) | 액세스 토큰 정의, 보안 접근 검사, 권한 조작, 감사 메시지 생성 |
| Lsass.exe | 유저 모드 | 로컬 보안 정책, 사용자 인증, 보안 감사 메시지를 이벤트 로그에 전송 |
| LsaIso.exe | VTL 1 Trustlet (Credential Guard 시) | 사용자 토큰 해시를 Lsass 메모리 대신 VTL 1에 격리 저장 |
| Lsass 정책 DB | 레지스트리 HKLM\SECURITY | 도메인 신뢰 관계, 접근 권한, 권한(Privilege), 감사 정책 |
| SAM 서비스 (Samsrv.dll) | Lsass 프로세스 내 | 로컬 사용자·그룹 데이터베이스 관리 |
| SAM 데이터베이스 | 레지스트리 HKLM\SAM | 로컬 사용자·그룹 정보, 비밀번호, 속성 |
| Active Directory (Ntdsa.dll) | Lsass 프로세스 내 | 도메인 사용자·그룹·컴퓨터 정보. 도메인 컨트롤러 간 복제 |
| Winlogon.exe | 유저 모드 | SAS(Ctrl+Alt+Del) 처리, 로그온 세션 관리, 첫 번째 프로세스 시작 |
| LogonUI.exe | 유저 모드 | 자격 증명 공급자(CP)를 로드해 로그온 UI 제공. Winlogon 보호용 별도 프로세스 |
| 자격 증명 공급자 (CP) | LogonUI 내 COM 오브젝트 | 비밀번호, 스마트카드, 생체인식 등 자격 증명 획득 |
| Netlogon | SvcHost 서비스 | 도메인 컨트롤러로의 보안 채널 설정, NTLM/Active Directory 인증 중계 |
| KSecDD (Ksecdd.sys) | 커널 모드 | EFS 등 커널 보안 컴포넌트가 Lsass와 통신하는 ALPC 인터페이스 제공 |
| AppLocker (AppId.sys) | 커널 드라이버 + 서비스 | 실행 가능한 파일·DLL·스크립트를 사용자·그룹별로 제한 |
 
SRM(커널)과 Lsass(유저 모드)는 ALPC로 통신. 시스템 초기화 후 연결이 고정되어 이후 다른 프로세스가 이 포트에 연결 불가 → 악의적 연결 차단
 
## 7-3 가상화 기반 보안 (VBS)
 
수백만 개의 서드파티 드라이버 중 하나라도 취약점이 있으면 커널을 완전히 신뢰할 수 없다. VBS는 이 문제를 하이퍼바이저를 통해 해결한다
 
VTL 구조 :
 
```
하이퍼바이저
├── VTL 1 : 보안 커널 + IUM(Isolated User Mode)
│   ├── HVCI (HyperVisor Code Integrity)  ← Device Guard 핵심
│   ├── LsaIso.exe (Trustlet)             ← Credential Guard 핵심
│   └── SKCI.DLL (Secure Kernel Code Integrity)
└── VTL 0 : 일반 NT 커널 + 드라이버 + 일반 앱
```
 
VTL 1 신뢰 전제 조건 : Secure Boot(펌웨어), 하이퍼바이저 무결성, IOMMU 정상 동작
 
## 7-4 Credential Guard
 
사용자 인증 자격증명을 VTL 1의 LsaIso.exe Trustlet에 격리해 커널 레벨 공격자도 탈취 불가하게 한다
 
보호 대상 자격증명 3가지 :
 
| 자격증명 | 설명 | 위험성 |
|---------|------|--------|
| Password | 사용자 인터랙티브 인증 기본 자격증명 | 가장 가치 높음. WDigest/RDP SSO용 평문 보관 필요 |
| NT OWF (NT One-Way Function) | NTLM 프로토콜용 비밀번호 MD4 해시 | 인터셉트 시 즉시 사용 가능, 비밀번호 복원 가능 |
| TGT (Ticket-Granting Ticket) | Kerberos 인증 티켓 + 키 | 인터셉트 시 도메인 리소스 무단 접근 가능 |
 
보호 방식 : Lsass가 KDC에서 받은 TGT/NTOWF를 ALPC로 LsaIso에 전달 → VTL 1에 암호화 저장 → Lsass는 암호화된 blob만 보유 → 서비스 티켓 생성 시 LsaIso가 처리
 
추가 보호 : Lsass를 PPL(Protected Process Light)로 실행 (RunAsPPL 레지스트리 값 = 1)
 
Credential Guard 한계 : WDigest·Terminal Services처럼 평문 비밀번호가 필요한 프로토콜에는 SSO 기능 비활성화가 유일한 해결책 → Windows Hello(생체인식)로 아예 비밀번호 입력을 없애는 것이 최선
 
## 7-5 Device Guard
 
기기 자체를 소프트웨어·하드웨어 기반 공격으로부터 보호하는 기능
 
핵심 기술 구성 :
 
- KMCS (Kernel-Mode Code Signing) : 커널 모드 드라이버 서명 강제
- UMCI (User-Mode Code Integrity) : 사용자 모드 이미지 서명 강제
- HVCI (HyperVisor Code Integrity) : SLAT(하이퍼바이저 2차 주소 변환)로 서명 검증을 VTL 1에서 수행
- CCI (Custom Code Integrity) : 기업 관리자가 정의한 커스텀 서명 정책
Device Guard의 주요 보장 (KMCS 강제 시) :
 
- 서명된 코드만 로드 가능 (커널이 침해되더라도)
- 로드된 서명 코드는 커널 자신도 수정 불가 (SLAT로 읽기 전용 보호)
- 동적 코드 할당 금지
- UEFI 런타임 코드 수정 불가
- W^X(Write XOR Execute) 모델 : SLAT가 실행 가능 페이지에 쓰기 불가, 쓰기 가능 페이지에 실행 불가
MBEC(Mode-Based Execution Control) : 하드웨어 지원 시 SLAT에 유저/커널 실행 비트를 추가 → ring 0 서명 코드만 커널 모드에서 실행 가능
 
## 7-6 객체 보호 (Protecting Objects)
 
Windows에서 보호 가능한 거의 모든 리소스는 오브젝트로 표현된다 (파일, 프로세스, 스레드, 레지스트리 키, 이벤트, 뮤텍스, 섹션 등)
 
스레드가 오브젝트에 접근할 때 `ObpGrantAccess` → `ObCheckObjectAccess` → `SeAccessCheck` 순으로 호출된다
 
보안 검사 3가지 입력 :
 
1. 스레드의 보안 자격증명 (토큰)
2. 요청하는 접근 유형 (읽기, 쓰기, 삭제 등)
3. 오브젝트의 보안 설정 (보안 서술자)
스레드는 오브젝트를 열 때 필요한 정확한 접근 마스크만 요청해야 한다 (PROCESS_ALL_ACCESS 남용 금지)
 
## 7-7 보안 식별자 (SID)
 
사용자, 그룹, 컴퓨터, 서비스를 고유하게 식별하는 가변 길이 숫자 값
 
형식 : `S-개정번호-식별자권한-하위권한값들-RID`
 
```
S-1-5-21-1463437245-1224812800-863842198-1128
│ │ │  └─────────── 하위권한값들 (도메인/컴퓨터 고유 부분)
│ │ └── 5 = Windows 보안 기관
│ └── 1 = 개정번호
└── S
```
 
주요 잘 알려진 SID :
 
| SID | 의미 |
|-----|------|
| S-1-1-0 | Everyone (익명 제외 모든 계정) |
| S-1-5-2 | Network (네트워크 로그온 사용자) |
| S-1-5-18 | SYSTEM |
| S-1-5-19 | Local Service |
| S-1-5-32-544 | Administrators 그룹 |
| S-1-15-2-1 | ALL_APP_PACKAGES |
 
RID 규칙 : Administrator = 500, Guest = 501. 일반 계정/그룹 = 1000부터 1씩 증가
 
## 7-8 무결성 레벨 (Integrity Levels)
 
같은 사용자 계정 내에서도 프로세스와 오브젝트를 격리하는 MIC(Mandatory Integrity Control) 메커니즘
 
| 레벨 | SID | 대표 사용처 |
|------|-----|-----------|
| Untrusted | S-1-16-0 | 익명 프로세스 |
| Low | S-1-16-4096 | 보호 모드 Internet Explorer |
| Medium | S-1-16-8192 | 일반 사용자 프로세스 (기본값) |
| High | S-1-16-12288 | 관리자 권한 상승 프로세스 |
| System | S-1-16-16384 | 시스템 서비스, 커널 |
| Protected Process | S-1-16-20480 | 보호 프로세스 |
 
AppContainer (UWP 앱) : 수준은 Low와 동일하지만 토큰에 AppContainer 플래그를 별도로 보유
 
오브젝트 필수 정책(Mandatory Policy) 세 가지 :
 
| 정책 | 설명 |
|------|------|
| No-Write-Up | 낮은 무결성 프로세스가 높은 무결성 오브젝트에 쓰기 불가 (기본값) |
| No-Read-Up | 낮은 무결성 프로세스가 높은 무결성 오브젝트를 읽기 불가 |
| No-Execute-Up | 낮은 무결성 프로세스가 높은 무결성 코드를 실행 불가 |
 
## 7-9 액세스 토큰 (Access Token)
 
프로세스 또는 스레드의 보안 컨텍스트를 표현하는 커널 오브젝트
 
토큰의 주요 내용 :
 
- 사용자 계정 SID
- 그룹 SID 목록 (소속 그룹)
- 권한(Privilege) 배열
- 무결성 레벨
- 세션 ID
- UAC 가상화 상태
- AppContainer 정보 (UWP 프로세스)
- 토큰 유형 : Primary(프로세스 토큰) vs Impersonation(스레드 가장 토큰)
- 기본 DACL
로그온 시 Lsass가 초기 토큰을 생성. 관리자 그룹·강력한 권한 보유 확인 후 Filtered Admin Token(제한된 토큰)과 Full Admin Token 두 개를 생성 → 일반 세션은 제한된 토큰으로 실행
 
토큰 필드는 커널 메모리에 있어 유저 모드에서 직접 수정 불가 (특정 시스템 콜과 적절한 접근 권한으로만 변경 가능)
 
## 7-10 계정 권한과 권한(Privilege)
 
### 계정 권한 (Account Rights)
 
특정 유형의 로그온(인터랙티브, 네트워크, 배치, 서비스, 터미널 서버)을 허용하거나 거부. SRM이 아닌 LSA가 로그온 시 확인. 토큰에 저장되지 않음
 
### 권한(Privilege)
 
특정 시스템 관련 작업을 수행하는 계정의 권한. 컴포넌트마다 각자 확인
 
주요 슈퍼 권한 (사실상 관리자 전권) :
 
| 권한 | 내부 이름 | 위험성 |
|------|---------|-------|
| Debug programs | SeDebugPrivilege | 모든 프로세스 메모리 직접 접근, 코드 주입 가능 |
| Take ownership | SeTakeOwnershipPrivilege | 모든 보안 가능한 오브젝트의 소유권 탈취 가능 |
| Restore files | SeRestorePrivilege | 시스템 파일 교체 가능 |
| Load/unload drivers | SeLoadDriverPrivilege | 임의 드라이버 로드 → System 계정 권한으로 코드 실행 |
| Create token | SeCreateTokenPrivilege | 임의 SID와 권한을 가진 토큰 생성 |
| Act as OS | SeTcbPrivilege | 신뢰된 Lsass 연결 설정 후 새 로그온 세션에 임의 SID 추가 가능 |
 
권한은 활성화/비활성화 가능. 권한 검사 성공 조건 : 토큰에 존재 AND 활성화 상태. `AdjustTokenPrivileges`로 전환
 
## 7-11 보안 서술자 (Security Descriptor)와 접근 제어
 
모든 보안 가능한 오브젝트는 보안 서술자를 가진다
 
보안 서술자 구성 :
 
- Owner SID : 오브젝트 소유자
- Group SID : 오브젝트 주 그룹
- DACL (Discretionary Access Control List) : 접근 허용/거부 ACE 목록. NULL DACL = 모두 허용, 빈 DACL = 모두 거부
- SACL (System Access Control List) : 감사 ACE 목록
ACE(Access Control Entry) 구성 : 유형(허용/거부/감사) + SID + 접근 마스크
 
```
보안 서술자
├── Owner SID
├── Group SID
├── DACL (임의 접근 제어 목록)
│   ├── ACE 1 : 허용, Alice, 읽기·쓰기
│   ├── ACE 2 : 거부, Bob, 쓰기
│   └── ACE 3 : 허용, Everyone, 읽기
└── SACL (시스템 접근 제어 목록)
    └── ACE 1 : 감사, 실패, Everyone, 쓰기
```
 
접근 검사 알고리즘 (SeAccessCheck) : 요청된 접근 마스크를 DACL의 ACE와 순서대로 비교. 거부 ACE가 허용 ACE보다 우선
 
## 7-12 보안 감사 (Security Auditing)
 
`SeSecurityPrivilege` : 보안 이벤트 로그 관리 권한
 
`SeAuditPrivilege` : 감사 메시지 생성 권한
 
오브젝트 접근 감사 : Audit Object Access 정책 활성화 + 오브젝트의 SACL에 감사 ACE 필요 → 접근 시도 시 성공/실패 원인을 포함한 감사 레코드 생성
 
전역 감사 정책 (Global Audit Policy) : 모든 파일 시스템 또는 레지스트리 키에 대해 SACL 없이도 감사 가능 → `AuditPol /resourceSACL` 명령으로 설정
 
## 7-13 AppContainer
 
Windows 8에서 도입된 UWP 프로세스용 보안 샌드박스. 원래 코드명은 LowBox
 
### AppContainer 보안 환경
 
AppContainer 토큰의 접근 검사 특징 :
 
- 일반 사용자·그룹 SID는 deny-only로 처리 → 사용자 자신의 SID로도 접근 불가
- NULL DACL도 거부로 처리 (일반적으로 NULL DACL = 모두 허용이지만 AppContainer에서는 예외)
- AppContainer SID 또는 capability SID에 명시적 허용 ACE가 있는 오브젝트만 접근 가능
### AppContainer 격리 환경 (4가지 Jail)
 
| 격리 영역 | 구현 |
|---------|------|
| 명명된 커널 오브젝트 | 오브젝트 관리자 namespace의 `\Sessions\x\AppContainerNamedObjects\{AppContainer SID}` |
| 전용 atom 테이블 | Win32k.sys가 전역 atom 테이블 접근 차단 → AppContainer별 전용 atom 테이블 |
| 전용 파일 시스템 | `%LOCALAPPDATA%\Packages\{패키지명}` 하위 디렉터리 (AppContainer SID로 ACL) |
| 전용 레지스트리 | `Settings.dat` 하이브 파일 (AppContainer SID로 ACL) |
 
### ALL APPLICATION PACKAGES SID (S-1-15-2-1)
 
모든 AppContainer 토큰에 자동으로 바인딩되는 그룹 SID. `%SystemRoot%\System32`, `HKLM\Software\Microsoft\Windows\CurrentVersion` 등 핵심 시스템 위치에 이 SID가 읽기·실행 허용 ACE로 등록되어 AppContainer가 기본 시스템 리소스에 접근 가능
 
Restricted AppContainer (Windows 10 1607~) : `PROC_THREAD_ATTRIBUTE_ALL_APPLICATION_PACKAGES_POLICY`로 ALL APPLICATION PACKAGES SID 접근도 차단. 대신 ALL RESTRICTED APPLICATION PACKAGES SID로 최소한의 시스템 접근 보장
 
### AppContainer Capabilities
 
UWP 개발자가 앱 매니페스트에 선언하는 기능 목록. 각 capability는 SID로 변환되어 토큰에 포함
 
capability SID 생성 방법 3가지 :
 
- 잘 알려진 capability : 하드코딩된 RID (S-1-15-3-N)
- 문자열 capability : 대문자 변환 후 SHA-2 해시 → 8개 RID
- 장치 capability : GUID → 4개 RID
capability 유형 :
 
| 유형 | 설명 |
|------|------|
| Capability | Windows 8 기본 capability (internetClient 등) |
| uap:Capability | 표준 스토어 앱 capability |
| rescap:Capability | 제한 capability (Microsoft 특별 승인 필요) |
| wincap:Capability | Windows 시스템 앱 전용 |
| DeviceCapability | 하드웨어 장치 접근 (microphone, camera 등) |
 
## 7-14 로그온
 
인터랙티브 로그온 관련 컴포넌트 : Winlogon ↔ LogonUI (CP 포함) ↔ Lsass ↔ 인증 패키지 ↔ SAM/Active Directory
 
### 로그온 흐름
 
1. 사용자가 SAS(Ctrl+Alt+Del) 입력
2. Win32k.sys가 RPC 메시지로 Winlogon에 통보
3. Winlogon이 LogonUI를 시작 → CP가 사용자 이름·비밀번호 획득
4. Winlogon이 LsaLogonUser를 호출해 인증 패키지에 자격 증명 전달
5. 인증 패키지가 SAM 또는 도메인 컨트롤러에서 계정 정보 확인
6. 인증 성공 시 Lsass가 사용자 토큰 생성 → Winlogon이 사용자 첫 프로세스 시작
### SAS 보안
 
- Win32k.sys가 Ctrl+Alt+Del를 전용 예약 → 어떤 앱도 가로챌 수 없음
- SetWindowsHookEx로 설치한 훅도 SAS에는 적용되지 않음
- Winlogon만 데스크톱을 잠그거나 잠금 해제 가능
### 두 가지 인증 패키지
 
| 패키지 | 사용 시나리오 |
|--------|-------------|
| MSV1_0 (Msv1_0.dll) | 로컬 컴퓨터 로그온, 도메인 컨트롤러 접근 불가 시 |
| Kerberos (Kerberos.dll) | Windows 도메인 멤버 컴퓨터. RFC 1510 기반 |
 
### Winlogon 초기화
 
1. 인터랙티브 윈도우 스테이션 생성 (SYSTEM SID만 접근 허용)
2. 두 개의 데스크톱 생성 : Application(일반 앱) + Winlogon(보안 데스크톱, Winlogon만 접근)
3. Lsass와 ALPC 연결 설정
4. Win32k에서 SAS·로그오프·워크스테이션 잠금 알림을 받는 RPC 메시지 서버 등록
## 7-15 UAC (User Account Control)
 
표준 사용자 권한으로 실행하는 것이 원칙. 필요 시에만 관리자 권한을 상승(Elevation)해 사용하는 메커니즘. Windows Vista에서 도입
 
UAC 주요 해결 과제 :
 
- 관리자 권한을 가정한 레거시 앱 호환성
- 표준 사용자가 가끔 필요한 관리자 작업 수행
관리자 계정 로그온 시 Filtered Admin Token (표준 사용자 수준)과 Full Admin Token 두 개 생성 → 일반 프로세스는 Filtered Token으로 실행
 
UAC는 보안 경계가 아닌 편의 기능이다. 악성코드가 표준 사용자 권한으로 실행 중이더라도 상승된 프로세스를 통해 관리자 권한을 얻을 수 없다는 보장이 없다
 
### 파일 시스템 및 레지스트리 가상화
 
레거시 앱이 시스템 전역 위치(%ProgramFiles%, %SystemRoot%, HKLM\Software 등)에 쓰려 하면 접근 거부 대신 사용자별 위치로 리다이렉트
 
파일 가상화 : `%LocalAppData%\VirtualStore`로 리다이렉트. `Luafv.sys` 파일 시스템 필터 드라이버 구현
 
레지스트리 가상화 : `HKCU\Software\Classes\VirtualStore`로 리다이렉트. 구성 관리자에서 구현
 
가상화 비활성화 조건 : 64비트 앱, 이미 관리자 권한으로 실행 중, 커널 모드 호출, 가장 중, UAC 호환 매니페스트 보유, 서비스
 
### Elevation (권한 상승)
 
앱이 관리자 권한을 요청하는 방법 :
 
- 요청된 실행 레벨(requestedExecutionLevel)을 매니페스트에 선언
  - asInvoker : 부모 토큰 그대로
  - highestAvailable : 가능한 가장 높은 권한
  - requireAdministrator : 관리자 권한 필수
UAC 프롬프트 종류 :
 
| 유형 | 조건 | 표시 내용 |
|------|------|---------|
| 동의 프롬프트 | 관리자가 관리자 권한 필요 앱 실행 | 허용 여부만 확인 |
| 자격증명 프롬프트 | 표준 사용자가 관리자 권한 필요 앱 실행 | 관리자 계정/비밀번호 입력 |
 
## 7-16 AppLocker
 
관리자가 특정 사용자·그룹에 대해 실행 가능한 파일을 제어하는 메커니즘 (Windows 8.1/10 Enterprise, Server 2012 이상)
 
SRP(Software Restriction Policies)의 개선판 : 사용자·그룹별 규칙 적용 가능, 감사 모드 지원
 
제어 가능한 파일 유형 : EXE, COM, DLL, OCX, MSI, MSP, 스크립트(PS1, BAT, VBS, JS)
 
규칙 기준 세 가지 :
 
| 기준 | 설명 |
|------|------|
| 서명 인증서 | 게시자, 제품명, 파일명, 버전 조합 |
| 디렉터리 경로 | 특정 경로 내 파일만 허용 |
| 파일 해시 | SHA-256 해시 일치 여부 |
 
AppLocker 동작 원리 :
 
- `AppId.sys` 드라이버가 `PsSetCreateProcessNotifyRoutineEx`로 프로세스 생성 알림 등록
- 프로세스 생성 시 AppID 속성 수집 → 토큰에 저장 → `SeSrpAccessCheck`로 규칙 검사
- DLL 제한 : 이미지 로더가 DLL 로드마다 AppId 드라이버에 DeviceIoControl 요청
- 스크립트 엔진·MSI 설치 관리자 : 유저 모드 SRP API 직접 호출
규칙은 조건부 ACE(Conditional ACE)와 AppID 속성으로 구현. `secpol.msc`나 그룹 정책으로 설정
 
## 7-17 커널 패치 보호 (KPP / PatchGuard)
 
드라이버가 시스템 콜 테이블 패치, 커널 이미지 수정 등 비지원 방식으로 커널을 변경하는 것을 감지하는 메커니즘. x64·ARM Windows에만 적용 (32비트 레거시 드라이버 호환성으로 x86 미적용)
 
PatchGuard는 보안 경계가 아니다 → 공격을 막거나 되돌리지 않음. 감지 시 BSOD로 시스템을 충돌시켜 추가 실행을 차단하는 것이 전부. CCTV + 경보 시스템으로 이해하면 적절하다
 
PatchGuard의 특징 :
 
- 보호 대상·검사 시점·검사 방법 비공개 및 난독화 (공격자의 예측 어렵게)
- 비결정적 실행 (비결정성이 신뢰성 높은 우회 방지)
- 감지 시 커널 모드 크래시 덤프 생성 → Microsoft에 자동 전송 (텔레메트리)
- 디버그 모드 + 원격 커널 디버거 연결 시 비활성화
보호 내용 (공개된 범위) :
 
- 커널 실행 코드와 데이터
- 시스템 서비스 서술자 테이블 (SSDT)
- 인터럽트 서술자 테이블 (IDT), GDT, MSR
- 프로세스 연결 리스트 (PsActiveProcessHead)
- 커널 스택
PatchGuard 대신 사용 가능한 지원 메커니즘 : 파일 시스템 미니 필터, 레지스트리 필터 알림, 프로세스 알림, 오브젝트 관리자 필터링, NDIS LWF, WFP 필터, ETW
 
## 7-18 HyperGuard
 
Windows 10 Anniversary Update(1607)에서 도입. VBS 활성화 시 VTL 1에서 VTL 0 커널을 감시하는 메커니즘
 
PatchGuard와의 차이점 :
 
| 특성 | PatchGuard | HyperGuard |
|------|-----------|-----------|
| 보안 경계 | 아님 (커널과 같은 레벨) | 진정한 보안 경계 (VTL 1이 VTL 0 감시) |
| 난독화 | 필수 | 불필요 (심볼 공개, 코드 비난독화) |
| 실행 방식 | 비결정적 | 결정적 |
| 감지 타이밍 | 지연 가능 | 즉시 감지 |
| 크래시 코드 | 다양 | 0x18C (HYPERGUARD_VIOLATION) |
 
HyperGuard의 추가 기능 :
 
- PatchGuard의 기능을 VTL 1에서 강화 보완
- PatchGuard 자체를 공격자로부터 보호
- NPIEP(Non-Privileged Instruction Execution Prevention) : SGDT/SIDT/SLDT 명령이 실제 커널 주소 대신 프로세서별 고유 가상 값 반환 → KASLR 우회 방지
디버깅 모드 + 원격 하이퍼바이저 디버거 연결 시 비활성화
