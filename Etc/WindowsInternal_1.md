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
휘발성 레지스터와 스택, TLS를 일컫는 말indows Intenal 부록 : https://aka.ms/winint7ed/downloads

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
### 성능 모니터 : 기본 프로그램, CPU의 ㅈ### 전용 가상 주소 공간
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
### Dependency Walker : DLL 의존성 확인
### PEview : exe 파일의 구성요소를 확인
### ProcessExplorer : 프로세스의 상세한 정보 ( dll, PID 등)

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
