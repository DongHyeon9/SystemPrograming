# 1장 줄임말

## OS - Operating System

운영체제. 챕터 도입부 첫 문장에서 "Windows operating system (OS)"로 등장. 이하 모든 논의의 대상

## API - Application Programming Interface

응용 프로그램과 OS 사이의 약속된 함수 집합
- 윈도우 API는 사용자 모드에서 OS 서비스를 호출하는 인터페이스
- CreateProcess, CreateFile 같은 C 스타일 함수 수천 개로 구성
- 32비트판은 Win32 API라고도 부름

## WDK - Windows Driver Kit

드라이버 개발 도구 패키지
- ntddk.h, wdm.h 등 핵심 헤더와 커널 지원 함수 문서 포함
- 체크드 빌드 커널·HAL도 여기서 제공

## SDK - Software Development Kit

Windows 애플리케이션 개발 도구 패키지
- Windows API 헤더·라이브러리 포함
- Debugging Tools for Windows(cdb, ntsd, kd, WinDbg) 포함

## MSDN - Microsoft Developer Network

마이크로소프트 개발자 지원 프로그램
- Windows API 문서, SDK/WDK 다운로드, 코드 샘플 제공
- 현재는 docs.microsoft.com으로 통합

## COM - Component Object Model

C 스타일 API의 명명 비일관성 문제를 해결하려 만든 인터페이스 기반 컴포넌트 모델
- 원래 Office 문서 간 데이터 교환(OLE)을 위해 탄생
- COM 1. 클라이언트는 vtable 인터페이스를 통해 객체와 통신 (C, C++, VB, .NET 등 언어 무관)
- COM 2. 구현체(DLL/EXE)는 런타임에 동적 로드
- DirectX, WIC, BITS 등이 COM으로 노출
- 추천 도서 Essential COM(Don Box, Addison-Wesley, 1998)

## OLE - Object Linking and Embedding

엑셀 차트를 워드 문서에 삽입하는 등 문서 간 객체 연결·삽입 기술
- COM의 직접적 전신
- 초기에는 DDE로 구현됐다가 COM 기반(OLE 2)으로 재설계됨

## DDE - Dynamic Data Exchange

Windows 메시지 기반의 구형 프로세스 간 데이터 교환 메커니즘
- OLE 초기 구현에 사용됐으나 본질적 한계로 COM으로 대체됨

## DLL - Dynamic Link Library

런타임에 동적으로 로드되는 공유 라이브러리
- 여러 프로세스가 단일 인메모리 복사본을 공유
- COM 서버 = *.DLL or *.exe
- Kernel32.dll, Ntdll.dll, Msvcrt.dll 등이 대표 예시

## WinRT - Windows Runtime

Windows 8에서 도입된 UWP(스토어 앱) 전용 API 및 런타임
- COM을 확장한 구조, 타입 메타데이터를 WINMD 파일에 저장
- 네임스페이스 계층, 일관된 명명, 프로그래밍 패턴으로 Win32보다 정교
- C++/CX, C#, JavaScript(WinJS)에서 소비 가능
- ※ Windows RT(ARM 전용 OS)와 혼동 주의

## CLR - Common Language Runtime

.NET 프레임워크의 실행 엔진
- JIT 컴파일러, 가비지 컬렉터, 타입 검증, 코드 접근 보안 등 제공
- COM 인프로세스 서버(DLL)로 구현됐으며 Windows API를 내부적으로 활용

## JIT - Just-In-Time

CLR이 CIL(중간 언어) 바이트코드를 실행 시점에 네이티브 CPU 기계어로 컴파일하는 방식

## CIL - Common Intermediate Language

.NET 컴파일러(C#, VB, F# 등)가 생성하는 플랫폼 독립 중간 언어
- CLR의 JIT 컴파일러가 최종 CPU 명령어로 변환

## FCL - .NET Framework Class Library

.NET이 제공하는 대규모 타입 라이브러리
- UI, 네트워킹, 데이터베이스 접근 등 클라이언트·서버 앱에 필요한 기능 제공

## UAC - User Account Control

프로세스 보안 컨텍스트(액세스 토큰) 설명 중 등장
- 관리자 그룹 사용자 로그인 시 표준 권한 토큰 + 관리자 권한 토큰 두 개 발급
- 필요 시에만 상승 권한을 사용하게 하는 보안 메커니즘

## WMI - Windows Management Instrumentation

드라이버·서비스가 성능·구성 정보를 게시하고 관리 명령을 수신하는 표준 프레임워크
- CIM/MOF 언어 기반
- PowerShell의 Get-WmiObject 등으로 조회

## TLS - Thread-Local Storage

스레드마다 독립적으로 유지되는 개인 저장 영역
- 서브시스템, 런타임 라이브러리, DLL에서 스레드 전용 데이터를 보관
- 파이버는 TLS를 공유하는 문제가 있어 FLS(Fiber-Local Storage)가 별도 존재

## UMS - User-Mode Scheduling

64비트 Windows 전용 경량 스레드 스케줄링
- 파이버의 장점을 제공하면서 커널에도 스레드 상태가 보임
- 블로킹 시스템 콜도 독립적으로 처리 가능
- 사용자 모드에서 컨텍스트 스위치 수행 → 커널 스케줄러 호출 감소

## AWE - Address Windowing Extensions

32비트 프로세스의 2GB 가상 공간 한계를 극복하는 API 메커니즘
- 최대 64GB 물리 메모리를 매핑·사용 가능
- 대용량 데이터베이스 서버에서 메모리 뷰를 직접 전환하는 방식으로 활용

## DEP - Data Execution Prevention

데이터 페이지를 non-executable로 표시해 데이터 영역에서의 코드 실행을 막는 하드웨어 기반 보호 기능
- CPU의 NX(No-Execute) / XD(Execute Disable) 비트 활용

## KMCS - Kernel-Mode Code Signing

64비트·ARM Windows 8.1 이상에서 모든 드라이버에 암호화 서명을 요구하는 정책
- Windows 10은 EV 인증서 + Microsoft 증명 서명(Attestation Signing)까지 요구
- Server 2016은 WHQL 인증까지 필요

## EV - Extended Validation

KMCS 정책 강화 맥락에서 등장
- SHA-2 기반 EV 하드웨어 인증서
- Windows 10에서 새 드라이버가 반드시 취득해야 하는 인증서 유형
- 일반 파일 기반 SHA-1 인증서를 대체

## WHQL - Windows Hardware Quality Labs

마이크로소프트의 공식 드라이버 인증 프로그램
- HCK(Hardware Compatibility Kit)로 호환성·보안·성능·안정성 검증
- Server 2016은 WHQL 서명 드라이버만 로드 허용

## VBS - Virtualization-Based Security

Hyper-V 하이퍼바이저를 이용해 VTL 1 격리 실행 환경을 만들고 구현하는 보안 기술 집합
- 포함 기술 : Device Guard(HVCI), Hyper Guard, Credential Guard, Application Guard, Host Guardian/Shielded Fabric

## TPM - Trusted Platform Module

시스템 무결성 측정·증명(attestation)을 위한 하드웨어 칩
- UEFI Secure Boot, BitLocker, Measured Boot 등에 사용
- v-TPM은 가상 머신용 소프트웨어 TPM

## VTL - Virtual Trust Level

하이퍼바이저가 제공하는 직교적 격리 계층
- VTL 0 = 일반 커널, VTL 1 = 보안 커널
- 권한 레벨(ring 0/3)과 독립적으로 동작
- VTL 0 커널조차 VTL 1 메모리를 조작할 수 없음

## UEFI - Unified Extensible Firmware Interface

BIOS를 대체하는 현대 펌웨어 인터페이스
- Secure Boot 필수 요구, 부트 소프트웨어의 서명 검증
- Windows 8 이상 인증 시스템에서 신뢰 체인의 시작점

## GUI - Graphical User Interface

그래픽 사용자 인터페이스
- Terminal Services 설명에서 "서버가 GUI를 원격 클라이언트에 전송"으로 등장

## CCITSE - Common Criteria for Information Technology Security Evaluation

정부·산업계의 OS 보안 등급 평가 기준
- Windows는 이 스펙의 요구사항을 충족하도록 설계됨
- 정부 조달 요건을 만족시킴

## BSTR - Basic String

COM 기반 API가 사용하는 유니코드 문자열 타입
- null-terminated 유니코드 배열
- 배열 시작 4바이트 앞에 문자열 길이(바이트)가 저장됨

## HSTRING - Handle String

Windows Runtime API가 사용하는 불변(immutable) 유니코드 문자열 타입
- WinRT는 유니코드 문자열만 사용

## WinDbg - Windows Debugger

GUI 기반 커널·사용자 모드 겸용 디버거
- DbgEng.dll 엔진 기반
- 커널 디버깅 시 !process, dt, !thread 등 확장 명령으로 내부 구조 탐색 가능

---

# 2장 줄임말

## NT - New Technology

챕터 도입부 "Windows NT"로 처음 등장
- 1989년 설계 요구사항 목록과 함께 소개
- Windows의 내부 커널 이름이자 제품 계보의 시작점
- 현재도 Ntoskrnl.exe, Ntdll.dll 등으로 NT 네이밍 유지

## POSIX - Portable Operating System Interface

Windows NT 설계 요구사항 중 하나로 등장
- IEEE 1003.1 UNIX 호환 인터페이스 표준
- NT는 POSIX 서브시스템으로 이를 지원했으나 Windows 8에서 폐지
- SUA(Subsystem for UNIX-based Applications)가 마지막 구현체

## SUA - Subsystem for UNIX-based Applications

환경 서브시스템 종류 설명 중 등장
- Windows 7 Ultimate/Enterprise 및 Server 2008 R2에서 제공된 강화 POSIX 서브시스템
- Windows 8에서 폐지

## WSL - Windows Subsystem for Linux

SUA 직후 Note 박스에서 등장
- Pico 프로바이더(Lxss.sys + LxCore.sys) 기반 Linux 바이너리 실행 환경
- PE 재컴파일 없이 ELF 실행 파일을 구동
- Windows 10 1607에서 개발자 베타로 도입

## HAL - Hardware Abstraction Layer

아키텍처 개요의 커널 모드 컴포넌트 목록에서 처음 정의
- Hal.dll
- 커널과 드라이버가 하드웨어를 직접 다루지 않고 HAL 루틴을 통해 접근하도록 추상화
- 인터럽트 컨트롤러, I/O 인터페이스, 멀티프로세서 통신 메커니즘 차이를 감춤

## GDI - Graphics Device Interface

Windows 서브시스템 컴포넌트 목록에서 등장
- Win32k.sys에 포함된 2D 그래픽 출력 라이브러리
- 애플리케이션이 하드웨어 세부사항 없이 디스플레이·프린터를 사용하도록 표준 인터페이스 제공

## SMP - Symmetric Multiprocessing

멀티프로세서 지원 섹션에서 정의
- 모든 CPU가 동일한 메모리 공간을 공유하고 어떤 CPU에서도 OS 코드·스레드가 실행될 수 있는 구조
- ASMP(비대칭 멀티프로세싱)와 대비됨

## ASMP - Asymmetric Multiprocessing

SMP 설명 직후 대비 개념으로 등장
- OS 커널 코드가 특정 CPU 하나에서만 실행되고 나머지는 사용자 코드만 실행하는 방식
- Windows는 ASMP를 사용하지 않음

## SMT - Simultaneous Multithreading

현대 멀티프로세서 시스템 유형 설명에서 등장
- 단일 물리 코어가 두 개 이상의 논리 프로세서를 제공하는 기술
- Intel 하이퍼-스레딩, AMD Zen SMT가 대표적
- Windows 스케줄러는 물리 코어와 논리 프로세서를 구분 관리

## NUMA - Non-Uniform Memory Access

SMT와 함께 네 가지 멀티프로세서 유형 중 하나로 등장
- CPU를 노드로 묶고 노드 로컬 메모리 접근이 더 빠른 아키텍처
- Windows는 SMP로 동작하면서 노드 친화도 스케줄링으로 성능 최적화

## ETW - Event Tracing for Windows

체크드 빌드를 대체하는 새 로깅 메커니즘(WPP 트레이싱, TraceLogging) 설명 맥락에서 처음 등장
- 두 메커니즘 모두 ETW 위에 구축됨
- WPA(Windows Performance Analyzer)로 분석

## VSM - Virtual Secure Mode

VBS와 동의어로 소개
- "You'll also sometimes see the term Virtual Secure Mode, or VSM, used."
- Windows 10 1607·Server 2016부터 지원 하드웨어에서 기본 활성화

## IUM - Isolated User Mode

VTL 1에서 실행되는 사용자 모드 환경
- 허용된 시스템 콜만 사용 가능 (I/O·그래픽·드라이버 통신 불가)
- Iumdll.dll(VTL 1판 Ntdll.dll), Iumbase.dll을 통해 노출
- Trustlet만 IUM에서 실행 가능

## SLAT - Second Level Address Translation

VBS 아키텍처 설명에서 Credential Guard·Device Guard 구현 수단으로 등장
- 하이퍼바이저가 게스트 물리 주소(GPA)를 실제 물리 주소(SPA)로 변환하는 하드웨어 기능
- Intel = EPT, AMD = NPT, ARM = Stage-2

## DMA - Direct Memory Access

SLAT 직후, I/O MMU 설명 맥락에서 등장
- CPU를 거치지 않고 디바이스가 직접 메모리에 접근하는 전송 방식
- IOMMU로 DMA 범위를 제한해 드라이버가 하이퍼바이저·보안 커널 물리 메모리에 접근하는 것을 차단

## IOMMU - I/O Memory Management Unit

DMA 직후 등장
- 디바이스의 DMA를 가상화·제한하는 하드웨어 유닛
- VBS에서 드라이버가 SLAT를 우회해 하이퍼바이저·보안 커널 메모리에 접근하는 것을 막는 핵심 장치

## NTFS - New Technology File System

주요 시스템 컴포넌트 탐구 영역 나열 중 처음 등장
- Windows 기본 파일 시스템
- 복구 가능성(트랜잭션 로그), 보안, EFS 암호화, 압축, 변경 저널 등 고급 기능 지원

## ALPC - Advanced Local Procedure Call

환경 서브시스템 DLL 동작 원리 설명 중 등장
- 동일 시스템 내 프로세스 간 메시지 전달 메커니즘
- RPC의 로컬 전송으로 사용

## PE - Portable Executable

Pico 프로바이더 / WSL 설명 중 등장
- Windows 실행 파일·DLL 포맷
- 이미지 헤더에 서브시스템 타입, 임포트/익스포트 테이블 등을 기록

## APC - Asynchronous Procedure Call

Ntdll.dll 지원 함수 목록에서 등장
- 특정 스레드에 비동기적으로 함수를 주입·실행하는 메커니즘
- 커널 APC와 유저 APC(ReadFileEx 완료 루틴 등)로 구분

## SRM - Security Reference Monitor

Executive 컴포넌트 목록에서 처음 정의
- 커널 내부의 보안 정책 집행기
- 객체 접근 시 호출자의 토큰과 ACL을 비교해 허용·거부를 결정하고 감사 이벤트를 기록

## PnP - Plug and Play

Executive 컴포넌트 목록에서 등장
- 디바이스 추가·제거를 자동으로 감지하고 드라이버를 로드·언로드하는 메커니즘
- 하드웨어 리소스(IRQ, I/O 포트, DMA, 메모리)를 각 디바이스에 할당

## WDM - Windows Driver Model

Executive 컴포넌트 목록의 WMI 루틴 설명 맥락에서 처음 정의
- Windows 2000에서 도입된 드라이버 모델
- PnP·전원 관리 지원
- Bus·Function·Filter 드라이버의 3계층 구조

## LPC - Local Procedure Call

Executive 지원 함수 목록에서 ALPC 섹션을 설명하면서 등장
- ALPC의 전신이자 기반 개념
- 동일 머신 내 프로세스 간 통신

## RPC - Remote Procedure Call

ALPC 설명에서 "ALPC is used as a local transport for remote procedure call (RPC)"로 등장
- 네트워크를 통해 원격 서버 함수를 호출하는 표준 프로토콜
- ALPC가 로컬 RPC 전송을 담당

## DPC - Deferred Procedure Call

커널 컨트롤 객체 목록에서 APC와 함께 등장
- 인터럽트 서비스 루틴이 우선도가 낮은 후속 처리를 지연 실행하는 메커니즘
- DISPATCH_LEVEL(IRQL 2)에서 실행
- KPRCB에 DPC 큐가 포함됨

## KPCR - Kernel Processor Control Region

커널 하드웨어 지원 섹션에서 처음 정의
- CPU별 전용 커널 데이터 구조
- IDT, TSS, GDT, 인터럽트 컨트롤러 상태 포함
- x32는 fs 레지스터, x64는 gs 레지스터를 통해 빠르게 접근
- !pcr 디버거 명령으로 조회

## IDT - Interrupt Descriptor Table

KPCR 설명에서 KPCR이 포함하는 항목으로 등장
- x86/x64 인터럽트 벡터 테이블
- 각 항목(게이트)이 하드웨어 인터럽트·예외 핸들러를 가리킴
- !idt 디버거 명령으로 조회 가능

## TSS - Task State Segment

KPCR 설명에서 IDT 직후 등장
- x86/x64 하드웨어 태스크 상태 세그먼트
- 커널 스택 포인터, IST(Interrupt Stack Table) 항목을 보관
- 인터럽트 처리 시 스택 전환에 사용

## GDT - Global Descriptor Table

KPCR 설명에서 TSS 직후 등장
- x86/x64 세그먼트 디스크립터 배열
- CS·DS·SS·FS·GS 등 세그먼트 기저 주소·한계·특권 레벨을 정의

## KPRCB - Kernel Processor Control Block

KPCR 설명에 이어 등장
- KPCR 안에 내장된 사적(private) 구조체
- 현재·다음·유휴 스레드 스케줄링 정보, DPC 큐, CPU 토폴로지·캐시 크기·시간 통계 보관
- WDK에 문서화 안 됨
- !prcb 명령으로 조회

## IRQL - Interrupt Request Level

!prcb 디버거 출력 예시 중 "Current IRQL -- 0"으로 처음 등장
- Windows 커널의 인터럽트 우선순위 레벨 (PASSIVE=0 ~ HIGH=31)
- 높은 IRQL은 낮은 IRQL 실행을 선점 가능
- 스핀락·DPC 처리 등과 긴밀히 연결됨

## APIC - Advanced Programmable Interrupt Controller

HAL 섹션에서 "processors require ACPI and APIC support"로 등장
- 현대 인터럽트 라우팅을 담당하는 컨트롤러
- 구형 PIC를 대체

## WDF - Windows Driver Foundation

드라이버 섹션 후반부에서 등장
- WDM의 복잡도를 숨기는 프레임워크
- KMDF(커널 모드)와 UMDF(사용자 모드)로 구성
- GitHub 오픈소스 공개

## KMDF - Kernel-Mode Driver Framework

WDF 설명에서 UMDF와 함께 등장
- WDM 위에서 동작하며 전원 관리·동기화 등 반복 코드를 줄여줌
- Windows 2000 SP4 이상 지원

## UMDF - User-Mode Driver Framework

KMDF와 함께 등장
- USB·고지연 프로토콜 디바이스(카메라, MP3 플레이어, 프린터 등)에 적합
- 드라이버 충돌 시 프로세스 재시작으로 복구 가능
- UMDF 2.0부터 KMDF와 동일한 객체 모델 공유

## SAM - Security Account Manager

Smss.exe 초기화 절차 26단계에서 등장
- 로컬 사용자·그룹 정보를 저장하는 레지스트리 하이브(HKLM\SAM)
- Lsass.exe가 로컬 인증 시 조회

## LSA - Local Security Authority

Wininit.exe 초기화 절차 13단계에서 등장
- 인증 정책 시행·로그온 처리를 담당하는 보안 서브시스템 서비스(Lsass.exe)
- 인증 패키지 DLL을 통해 암호 확인, 토큰 생성 수행

## SCM - Service Control Manager

Wininit.exe 초기화 절차 14단계에서 처음 정의
- Services.exe 프로세스
- 서비스 시작·중지·재시작 및 상태 보고를 처리
- HKLM\SYSTEM\CurrentControlSet\Services 레지스트리 기반 동작

## SAS - Secure Attention Sequence

Winlogon.exe 설명에서 처음 정의
- Ctrl+Alt+Delete 키 조합
- 사용자 모드 애플리케이션이 가로챌 수 없어 로그인 화면 스푸핑을 방지
- SAS 감지 시 Winlogon이 LogonUI.exe를 실행해 자격 증명 공급자를 초기화

---

# 3장 줄임말

## RPC - Remote Procedure Call

CreateProcessWithLogonW / WithTokenW가 Secondary Logon 서비스에 RPC로 실제 프로세스 생성을 위임함을 설명할 때 등장
- 네트워크를 통해 원격 서버 함수를 호출하는 표준 프로토콜

## PE - Portable Executable

CreateProcess*가 PE 파일, 배치 파일, 16비트 COM 애플리케이션만 직접 받는다는 설명에서 등장
- Windows 실행 파일(.exe, .dll) 포맷의 공식 명칭

## EXE - Executable File

PE 설명 직후 "EXE extension is not strictly required"로 등장
- Windows 실행 파일 확장자

## UWP - Universal Windows Platform

모던 앱(스토어 앱)을 부르는 공식 명칭으로 처음 정의
- CreateProcess만으로 생성 불가 → IApplicationActivationManager COM 인터페이스 필요
- 모든 UWP 앱은 잡(Job) 안에서 실행됨

## EPROCESS - Executive Process Structure

프로세스 내부 구조 섹션에서 처음 정의
- Windows가 각 프로세스를 표현하는 커널 내부 핵심 데이터 구조
- 첫 번째 멤버 = Pcb (KPROCESS)
- KTHREAD, PEB, VAD 등을 포함 또는 포인팅

## ETHREAD - Executive Thread Structure

EPROCESS 설명과 함께 등장
- 각 스레드를 표현하는 커널 내부 핵심 데이터 구조
- 첫 번째 멤버 = Tcb (KTHREAD)
- 시스템 주소 공간에 존재 (TEB 제외)

## PEB - Process Environment Block

EPROCESS 관련 데이터 구조 중 유일하게 프로세스(유저) 주소 공간에 존재하는 구조체
- 이미지 로더, DLL 목록, 환경 변수 등 유저모드 코드가 접근하는 정보 포함

## KPROCESS - Kernel Process Structure

EPROCESS의 첫 번째 멤버인 Pcb(Process Control Block)의 실제 타입으로 정의
- 디스패처·스케줄러·인터럽트·시간 계산 코드가 EPROCESS 대신 KPROCESS를 사용 → 계층 분리

## CSR - Client/Server Runtime (Subsystem)

CSR_PROCESS 구조체 설명에서 처음 등장
- Windows 서브시스템 프로세스(Csrss.exe)가 각 Windows 프로그램 프로세스에 대해 병렬로 유지하는 구조체의 접두사

## PMP - Protected Media Path

보호 프로세스 섹션에서 처음 정의
- Blu-ray 등 고품질 디지털 콘텐츠의 안전한 재생을 위한 Windows 구성 요소
- Media Foundation API를 통해 보호 프로세스를 활용

## WER - Windows Error Reporting

WER 클라이언트 프로세스(Werfaultsecure.exe)가 보호 프로세스로 실행되는 이유를 설명할 때 처음 정의
- 보호 프로세스 크래시 시 접근이 필요하기 때문

## PPL - Protected Process Light

보호 프로세스의 확장 모델
- Signer 값에 따라 보호 수준이 차등화됨
- smss.exe, csrss.exe, services.exe, wininit.exe 등이 WinTcb-Lite로 실행

| Signer | 수준 | 사용 예 |
|--------|------|---------|
| WinSystem | 최고 | System 프로세스, Memory Compression |
| WinTcb | 높음 | smss.exe, csrss.exe, services.exe, wininit.exe |
| Windows | 중간 | sppsvc.exe |
| Antimalware | 중간 | MsMpEng.exe (Windows Defender) |

## DRM - Digital Rights Management

PPL 도입 배경 설명에서 처음 등장
- 멀티미디어 DRM → Windows 라이선싱 DRM → Windows Store DRM으로 확장

## AM - Anti-Malware

악성 프로세스가 자신을 보호 프로세스로 위장해 안티멀웨어(AM) 소프트웨어를 피하는 문제 설명에서 등장
- Code Integrity 모듈의 EKU OID 검증으로 이를 방지

## EKU - Enhanced Key Usage

디지털 코드 서명 인증서에 인코딩되는 특수 OID 필드
- PPL 사이너 결정에 사용
- 예: Windows System Component Verification EKU(1.3.6.1.4.1.311.10.3.6)가 있어야 WinSystem 사이너 획득 가능

## PID - Process Identifier

Process Explorer가 보호 프로세스를 찾을 때 핸들별 PID 정보를 조회하는 맥락에서 재등장
- 시스템 내 프로세스의 고유 식별 번호

## ELAM - Early-Launch Anti-Malware

서드파티 PPL 지원 섹션에서 처음 정의
- 부팅 초기에 로드되는 안티멀웨어 드라이버
- Microsoft 발급 특수 인증서 필요
- ELAMCERTIFICATEINFO PE 리소스 섹션을 통해 AM 서비스가 PPL 수준 획득

## OID - Object Identifier

EKU 설명 직후 "EKUs (identified by OID)"로 등장
- ASN.1 기반의 전역 고유 식별자 번호 체계
- 디지털 인증서의 EKU를 식별하는 데 사용

## TEB - Thread Environment Block

미니멀 프로세스 설명에서 처음 정의
- 스레드 전용 유저모드 데이터 구조
- PEB와 마찬가지로 프로세스(유저) 주소 공간에 존재

## KTHREAD - Kernel Thread Structure

스레드 생성 9단계에서 처음 정의
- ETHREAD의 첫 번째 멤버(Tcb)
- 스케줄링·동기화·시간 계산 데이터를 보관하는 커널 스레드 구조체

## ETW - Event Tracing for Windows

피코 프로바이더가 받는 콜백 목록에서 처음 등장
- 커널·유저모드 컴포넌트의 시스템 전체 이벤트 추적 인프라
- WPP 트레이싱, TraceLogging이 ETW 위에 구축됨

## SVN - Security Version Number

트러스틀릿 ID 목록에서 등장
- 트러스틀릿이 서명·암호화 데이터의 출처를 강력히 증명할 때 사용하는 버전 번호
- Credential Guard, Key Guard의 AES256/GCM 암호화에 활용

## SDF - Secure Device Framework

트러스틀릿이 보안 하드웨어 디바이스를 유저모드 드라이버로 다루기 위한 프레임워크
- SDFHost.dll에 포함
- Secure Biometrics(Windows Hello용 지문 센서, 웹캠 등)에 활용

## FIPS - Federal Information Processing Standard

트러스틀릿 암호화 서비스(IumCrypto) 설명에서 등장
- 미국 연방 정보 처리 표준
- Secure Kernel의 FIPS 모드 여부를 트러스틀릿이 조회 가능

## RNG - Random Number Generator

FIPS 직후 등장
- Secure Kernel이 IUM 전용으로 생성하는 난수 시드
- 유저모드에서는 접근 불가

## SEH - Structured Exception Handling

트러스틀릿 접근 가능 시스템 콜 목록 마지막에 등장
- Windows의 구조화 예외 처리 메커니즘
- 트러스틀릿도 Exception and Continue API를 통해 SEH 지원

## VDM - Virtual DOS Machine

CreateProcess 1단계에서 잡(Job) 내 프로세스가 별도 VDM을 요청할 경우 플래그가 무시된다는 설명에서 등장
- 16비트 MS-DOS 애플리케이션 실행을 위한 가상 환경

## TID - Thread Identifier

프로세스 속성 목록에서 "Returns the TID and PID of the initial thread"로 등장
- 프로세스 생성 결과로 반환되는 초기 스레드의 고유 ID

## NX - No-Execute

프로세스 속성의 완화 옵션 나열에서 등장
- CPU 하드웨어 기능으로 데이터 페이지를 실행 불가로 표시
- DEP의 하드웨어 구현체
- AMD는 NX, Intel은 XD(Execute Disable)라 부름

## IFEO - Image File Execution Options

CreateProcess 스테이지 2와 이미지 로더 초기화 과정에서 등장
- HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options
- 실행 파일별 디버거 지정, 힙 디버그 옵션, 전역 플래그 등 설정

## VAD - Virtual Address Descriptor

프로세스 주소 공간 구성 단계 설명에서 처음 등장
- 메모리 관리자가 프로세스의 가상 주소 사용 현황을 추적하는 트리 자료구조
- VirtualAlloc 등으로 예약된 범위가 VAD로 관리됨

## NLS - National Language Support

이미지 로더 초기화 7단계에서 등장
- 다국어 지원을 위한 문자 코드 변환·정렬·로케일 처리 테이블

## IRQL - Interrupt Request Level

스레드 생성 최종 단계에서 "KiStartUserThread lowers the thread's IRQL from DPC level to APC level"로 등장
- 인터럽트 우선순위 레벨 (PASSIVE=0 ~ HIGH=31)
- 새 스레드가 DPC 레벨에서 APC 레벨로 낮아진 후 PspUserThreadStartup 호출

## DPC - Deferred Procedure Call

스레드 시작 코드에서 "from deferred procedure call (DPC) level"으로 처음 정의
- 인터럽트 서비스 루틴이 후속 처리를 지연 실행하는 메커니즘
- DISPATCH_LEVEL(IRQL 2)에서 실행

## APC - Asynchronous Procedure Call

DPC 직후 "to APC level"로 처음 정의
- 특정 스레드에 비동기적으로 함수를 주입·실행하는 메커니즘
- 커널 APC(시스템 전용)와 유저 APC(ReadFileEx 완료 루틴 등)로 구분

## FLS - Fiber-Local Storage

이미지 로더가 초기화하는 항목 목록에서 처음 정의
- 파이버(Fiber) 전용 개인 저장 영역
- TLS와 별개로 존재

## SxS - Side-by-Side

이미지 로더 역할 설명에서 등장
- Windows Side-by-Side 어셈블리 기술
- 같은 DLL의 여러 버전이 동시에 공존할 수 있게 하는 매니페스트 기반 DLL 격리 메커니즘
- DLL Hell 해결

## MUI - Multilingual User Interface

이미지 로더가 처리하는 파일 목록에서 등장
- 다국어 UI 리소스 파일
- 단일 바이너리가 여러 언어 UI를 지원할 수 있게 해주는 Windows 국제화 메커니즘

## IAT - Import Address Table

이미지 로더 주요 역할에서 처음 정의
- PE 실행 파일이 사용하는 외부 DLL 함수 주소 테이블
- 로더가 로드 시 실제 함수 주소로 채움

## MRDATA - Mutable Read-Only Data

이미지 로더 초기화 5단계에서 처음 정의
- 보안 관련 전역 변수를 저장하는 특수 힙 섹션
- 익스플로잇에 의한 수정을 방지

## CFG - Control Flow Guard

이미지 로더 초기화 12단계에서 처음 정의
- 간접 호출 대상의 유효성을 런타임에 검증해 ROP(Return-Oriented Programming) 공격을 방지

## ASLR - Address Space Layout Randomization

프로세스 생성 7단계 완료 이후 주소 공간 레이아웃 설명에서 처음 등장
- 실행 파일, DLL, 힙, 스택 주소를 무작위화
- 커널 주소 공간도 ASLR 적용
- 공격자의 특정 주소 예측을 어렵게 함

## DAM - Desktop Activity Moderator

잡(Job)의 활용 목적 나열에서 처음 정의
- Win32 앱과 서비스의 스로틀링, 타이머 가상화, 타이머 동결 등 유휴 동작을 관리
- 잡을 통해 구현

## DFSS - Dynamic Fair-Share Scheduling

DAM 직후 잡 활용 목적 나열에서 처음 정의
- 잡을 통해 스케줄링 그룹을 정의하고 CPU 자원을 공정하게 분배하는 동적 스케줄링 메커니즘

## DSCP - Differentiated Services Code Point

잡의 네트워크 대역폭 제한 설명에서 처음 등장
- QoS 목적으로 잡이 전송하는 각 네트워크 패킷에 태그를 붙이는 데 사용

## QoS - Quality of Service

DSCP 직후 등장
- 네트워크 트래픽의 우선순위와 대역폭을 제어해 서비스 품질을 보장하는 기술

## IOPS - Input/Output Operations Per Second

잡의 디스크 I/O 대역폭 제한 설명에서 처음 등장
- 디스크 I/O를 대역폭(바이트) 또는 초당 I/O 작업 횟수(IOPS) 기준으로 제한 가능

## WIM - Windows Imaging Format

Windows 컨테이너(서버 사일로) 구성 요소 설명에서 처음 정의
- 컨테이너가 사용하는 별도 OS 복사본 파일 포맷
- Microsoft는 Server Core 이미지와 Nano Server 이미지를 WIM으로 제공

## VReg - Virtual Registry

컨테이너 격리 구성 요소에서 처음 정의
- 컨테이너가 임시 레지스트리 하이브 세트와 네임스페이스 격리를 제공하는 커널 컴포넌트
- 컨테이너 종료 시 변경 사항 제거

## LUID - Locally Unique Identifier

컨테이너 로그온 세션 설명에서 처음 정의
- 시스템 재부팅 내에서만 고유한 64비트 식별자
- 로그온 세션, 권한(Privilege) 등을 식별하는 데 사용

## SLS - Silo-Local Storage

사일로 컨텍스트 설명에서 처음 정의
- 서버 사일로마다 독립적으로 유지되는 저장 배열
- TLS(스레드 로컬 스토리지)와 유사한 개념
- 같은 슬롯 인덱스에서 사일로마다 다른 포인터를 저장

---

# 4장 줄임말

## ETHREAD - Executive Thread Structure

챕터 도입부 데이터 구조 섹션에서 첫 정의
- "executive thread object encapsulates an ETHREAD structure"
- KTHREAD를 첫 번째 멤버로 포함
- 시스템 주소 공간에 존재 (TEB 제외)

## TIB - Thread Information Block

TEB 구조 설명에서 처음 정의
- TEB의 헤더 부분
- OS/2 및 Win9x 애플리케이션과의 호환성을 위해 존재
- 예외·스택 정보를 담는 작은 구조체

## WSRM - Windows System Resource Manager

우선순위 레벨 섹션 이후 별도 박스에서 처음 정의
- Windows Server 2012 R2 이상에 포함된 옵션 컴포넌트
- 프로세스별 CPU 사용률·친화도·메모리 한도 정책을 설정
- 리소스 사용 보고서 생성 가능

## SMT - Symmetric Multithreading

프로세서 선택 섹션에서 처음 정의
- 단일 물리 코어가 두 개 이상의 논리 프로세서를 제공하는 기술
- Intel 하이퍼-스레딩, AMD SMT 등
- 쿼텀 만료 시 스케줄러가 SMT 세트 상태를 고려해 프로세서를 선택

---

# 5장 줄임말

## TLB - Translation Lookaside Buffer

대형 페이지 장점 설명에서 처음 정의
- CPU 내부의 가상→물리 주소 변환 캐시
- 용량이 매우 작아 대형 페이지를 사용하면 동일한 가상 범위를 더 적은 TLB 엔트리로 커버
- TLB 미스 → 페이지 테이블 워크(page table walk) 발생

## PFN - Page Frame Number

메모리 관리자가 사용하는 메모리 구조 목록에서 처음 등장
- 물리 메모리의 각 페이지를 식별하는 번호
- PFN 데이터베이스 : 시스템의 모든 물리 페이지 상태(제로/빈/대기/변경/불량 등)를 추적

## ACL - Access Control List

공유 메모리 섹션 보호 설명에서 처음 등장
- "Shared memory section objects have standard Windows access control lists (ACLs)"
- 파일·프린터 등 객체에 부착된 접근 허용/거부 목록

## DEP - Data Execution Prevention

메모리 보호 섹션에서 처음 정의
- "Data Execution Prevention (DEP), or no-execute (NX) page protection"
- 데이터 영역에서 코드가 실행되는 것을 방지
- PE 헤더의 IMAGE_DLLCHARACTERISTICS_NX_COMPAT 플래그로 활성화

## MMU - Memory Management Unit

주소 변환 섹션에서 처음 정의
- "ignored by the memory management unit (MMU) inside the CPU"
- CPU 내부에서 가상→물리 주소 변환을 수행하는 하드웨어
- PTE(페이지 테이블 엔트리)를 해석하여 변환

## SID - Security Identifier

ASLR 관련 레지스트리 키 설명에서 처음 등장
- "each one representing a given user SID"
- 사용자, 그룹, 컴퓨터 계정을 고유하게 식별하는 가변 길이 구조체
- 접근 제어(ACL)에서 주체를 식별하는 데 사용

## MFT - Master File Table

워킹셋 페이지 폴트 종류 설명에서 처음 등장
- "faults taken on the NTFS master file table (MFT) metadata file"
- NTFS 파일 시스템의 핵심 메타데이터 구조
- 볼륨의 모든 파일·디렉터리 정보를 레코드로 저장

## SGX - Software Guard Extensions

메모리 엔클레이브 섹션에서 처음 정의
- "Intel Software Guard Extensions (SGX)"
- 프로세서 하드웨어가 직접 보호하는 메모리 엔클레이브 기술
- OS·하이퍼바이저도 내부를 읽을 수 없는 강력한 격리 제공

## AES - Advanced Encryption Standard

SuperFetch 데이터 저장 설명에서 처음 등장
- "written using Advanced Encryption Standard (AES) encryption"
- SuperFetch가 디스크에 기록하는 추적 데이터를 부팅 세션 키로 AES 암호화
- 대칭 키 암호화 표준

## COW - Copy-on-Write

공유 메모리 DLL 설명에서 처음 정의
- 공유 페이지를 수정하려 할 때 OS가 해당 프로세스만의 개인 복사본을 만드는 메커니즘
- DLL 코드는 여러 프로세스가 공유하지만, 쓰기 가능 데이터 페이지는 COW로 분리

## EPT - Extended Page Tables

SLAT의 Intel 구현 이름
- 게스트 OS 페이지 테이블과 별도로 동작하는 두 번째 주소 변환 계층
- 하이퍼바이저가 게스트 물리 주소(GPA)를 실제 물리 주소(SPA)로 변환

## IPC - Inter-Process Communication

프로세스 간 데이터를 공유하기 위한 다양한 메커니즘의 총칭
- 공유 메모리 섹션, 파이프, ALPC, RPC 등이 모두 IPC의 구현 형태

---
 
# 6장 줄임말
 
## FAT - File Allocation Table
 
파일 시스템 목록 나열에서 처음 등장
- Windows가 지원하는 설치형 파일 시스템 중 하나
- FAT12, FAT16, FAT32, exFAT 변형 포함
- USB 드라이브, 플래시 카드 등에 주로 사용
- NTFS 대비 보안·복구·대용량 기능 없음
 
## CDFS - CD-ROM File System
 
FAT 직후 파일 시스템 목록에서 처음 정의
- CD-ROM 미디어 전용 파일 시스템
- ISO 9660 표준 기반
- 읽기 전용 파일 시스템
 
## UDF - Universal Disk Format
 
CDFS 직후 등장
- CD, DVD, Blu-ray 등 광학 미디어를 위한 표준 파일 시스템
- CDFS보다 더 많은 기능 지원 (긴 파일명, 유니코드 등)
- DVD-RAM 같은 쓰기 가능 광학 미디어 지원
 
## ReFS - Resilient File System
 
UDF 직후 등장. "the Resilient File System (ReFS)"로 처음 정의
- Windows Server 2012에서 도입된 최신 파일 시스템
- 데이터 무결성 자동 검증 (저장 시 체크섬 계산)
- 대용량 볼륨·파일 지원, 가상화 환경 최적화
- 전체 NTFS 기능(예: 압축)은 지원하지 않음
 
## INF - Device Installation File
 
드라이버 설치 파일을 설명하는 섹션에서 처음 정의
- 확장자 .inf. 하드웨어와 드라이버의 연결 정보를 담은 스크립트 형식 파일
- 장치 ID, 드라이버 파일 위치, 레지스트리 수정 사항, 의존성 정보 포함
- .cat 파일에 디지털 서명 저장 → 드라이버 변조 방지
 
## IRP - I/O Request Packet
 
I/O 관리자 섹션에서 처음 정의
- I/O 요청을 표현하는 핵심 데이터 구조
- I/O 시스템이 패킷 기반으로 동작하는 핵심 단위
- 주 함수 코드 (IRP_MJ_READ 등), 상태, MDL 포인터, I/O 스택 위치 배열을 포함
- Fast I/O만 IRP를 사용하지 않는 예외
- 프로세서별 룩어사이드 리스트에서 할당 (소형 1개, 중형 4개, 대형 14개 스택 위치)
 
## DIRQL - Device Interrupt Request Level
 
IRQL 레벨 목록에서 처음 정의
- 하드웨어 인터럽트에 할당되는 IRQL 수준
- x86에서 3~26, x64·ARM에서 3~12
- 항상 DISPATCH_LEVEL(2)보다 높으므로 IRQL 2의 제약이 모두 적용됨 (페이지 폴트 불가, 스케줄러 작동 불가)
- ISR이 실행되는 IRQL 수준
 
## ISR - Interrupt Service Routine
 
DIRQL 설명 직후 처음 정의
- 하드웨어 인터럽트 발생 시 커널 트랩 디스패처가 제어를 넘기는 드라이버 루틴
- DIRQL에서 실행 → 최소한의 작업만 수행 (장치 상태 읽기, 인터럽트 신호 해제)
- 나머지 처리는 DPC로 위임
- 인터럽트 기반 장치에만 존재 (파일 시스템 드라이버에는 없음)
- 특정 스레드 컨텍스트에 의존하는 코드 작성 불가 (임의 스레드에서 실행됨)
 
## USB - Universal Serial Bus
 
유저 모드 드라이버 설명에서 처음 등장
- "a kernel-mode port driver such as the universal serial bus (USB) printer port driver"로 등장
- 범용 직렬 버스. 표준화된 플러그 앤 플레이 주변기기 연결 인터페이스
- UMDF의 적합한 대상 프로토콜 중 하나
 
## PCMCIA - Personal Computer Memory Card International Association
 
WDM 버스 드라이버 예시 목록에서 처음 등장
- 노트북 확장 카드 표준 인터페이스 단체 및 표준 명칭
- 현재는 PC Card 또는 ExpressCard로 발전
- PCI, USB, IEEE 1394와 함께 버스 드라이버 예시로 언급
 
## PCI - Peripheral Component Interconnect
 
PCMCIA 직후 버스 드라이버 예시 목록에서 등장
- 마더보드와 확장 카드 간의 고속 로컬 버스 표준
- PCIe(PCI Express)로 발전
- 대부분의 데스크톱·서버의 주 확장 버스
 
## IEEE - Institute of Electrical and Electronics Engineers
 
PCI 직후 "IEEE 1394"로 처음 등장
- 전기전자공학자협회
- IEEE 1394 = FireWire. 고속 직렬 버스 표준. 비디오 카메라, 외장 스토리지 등에 사용
- UMDF의 지원 프로토콜 중 하나
 
## PDO - Physical Device Object
 
WDM 디바이스 노드 설명에서 처음 정의
- 버스 드라이버가 PnP 관리자의 지시로 생성하는 장치 오브젝트
- 장치의 물리적 인터페이스 표현
- 디바이스 스택에서 항상 맨 아래 위치 (필수)
- 버스 드라이버가 생성
 
## FDO - Functional Device Object
 
PDO 설명 직후 처음 정의
- 기능 드라이버(Function Driver)가 생성하는 장치 오브젝트
- 장치의 논리적 인터페이스 표현. 장치 기능을 가장 잘 아는 드라이버가 생성
- 디바이스 스택에서 하나만 존재 (필수)
- 애플리케이션이 실제로 통신하는 인터페이스 제공
 
## SATA - Serial Advanced Technology Attachment
 
포트 드라이버 예시에서 처음 등장
- "processing of an I/O request specific to a type of I/O port, such as SATA"로 등장
- 하드 디스크, SSD를 연결하는 직렬 인터페이스 표준
- 포트 드라이버(Ataport.sys)가 관리
- PATA(병렬 ATA)를 대체
 
## NDIS - Network Driver Interface Specification
 
포트 드라이버 설명에서 처음 정의
- "Network Driver Interface Specification (NDIS) is the network 'port' driver"로 등장
- 네트워크 어댑터 드라이버와 프로토콜 스택 사이의 표준 인터페이스
- 미니포트 드라이버(네트워크 어댑터 제조사 작성)와 NDIS 사이의 추상화 계층
- TCP/IP 등 프로토콜 드라이버가 NDIS를 통해 하드웨어에 접근
 
## MDL - Memory Descriptor List
 
I/O 요청 패킷 섹션에서 처음 정의
- "a memory descriptor list (MDL)...represents information for a buffer in physical memory"로 정의
- 물리 메모리의 버퍼를 서술하는 데이터 구조
- 드라이버가 DMA 전송, 직접 I/O 버퍼 접근 시 사용
- 가상 주소 대신 물리 페이지 프레임 번호 목록으로 버퍼 표현
 
## DDI - Device Driver Interface
 
I/O 완료 루틴 설명에서 처음 등장
- "a driver can register with the IoSetCompletionRoutine(Ex) DDI"로 등장
- 드라이버가 커널 I/O 관리자와 상호작용하기 위한 함수 인터페이스 집합
- WDK 문서에 정의됨
- Universal DDI : 여러 Windows 플랫폼(IoT, Mobile, 데스크톱)에서 공통으로 사용 가능한 DDI 집합
 
## HID - Human Interface Device
 
디바이스 노드 덤프 실험에서 처음 등장
- "DeviceInst is 'HID\\MSHW0029'"로 등장
- 마우스, 키보드, 게임 컨트롤러 등 사람이 직접 조작하는 입력 장치 클래스
- USB HID 클래스로 표준화 → 제조사별 드라이버 없이도 기본 동작
- UMDF의 지원 프로토콜 중 하나
 
## ACPI - Advanced Configuration and Power Interface
 
I/O 스택 덤프 실험에서 처음 등장 (\\Driver\\ACPI)
- 전원 관리 섹션에서 "Windows power-management capabilities require hardware that complies with the Advanced Configuration and Power Interface (ACPI) specification"으로 정의
- 하드웨어 리소스 구성, 전원 관리, 열 관리, 플러그 앤 플레이를 위한 개방형 표준
- UEFI의 일부. 시스템 전원 상태(S0~S5)와 장치 전원 상태(D0~D3) 정의
- BIOS의 고정된 전원·구성 관리를 OS가 동적으로 제어할 수 있도록 대체
 
## MUP - Multiple UNC Provider
 
I/O 취소 섹션에서 처음 등장
- "drivers that manage network file systems (for example, MUP, DFS, and SMB)"로 등장
- UNC 기반 경로(\\서버명\공유명)를 원격 리소스로 연결하는 드라이버
- Start 값 auto-start(2)를 사용하는 비PnP 파일 시스템 드라이버의 대표 예시
 
## SCSI - Small Computer System Interface
 
포트 드라이버 목록에서 처음 등장
- "responsible for all I/Os on a specific port, such as ATA, SCSI, or USB"로 등장
- 스토리지 장치(HDD, 테이프, 광학 드라이브, 스캐너 등)를 연결하는 병렬 인터페이스 표준
- 현재는 SAS(Serial Attached SCSI)로 발전
- 미니포트 드라이버 개념의 전통적 구현체
 
## ISA - Industry Standard Architecture
 
레거시 장치 설명에서 처음 등장
- "a device that isn't Plug and Play–compatible...such as a legacy ISA sound card"로 등장
- 구형 PC 확장 버스 표준. PnP를 지원하지 않는 대표적 레거시 인터페이스
- ISA 사운드 카드 등은 자동 감지 불가 → 일부 PnP 기능 사용 불가
 
## UNC - Universal Naming Convention
 
드라이버 로드 순서 설명에서 처음 정의
- "the Multiple Universal Naming Convention (UNC) Provider (MUP) driver"로 정의
- 네트워크 공유 자원을 \\서버명\공유명 형식으로 표현하는 경로 규칙
- MUP 드라이버가 UNC 경로를 실제 네트워크 프로바이더(SMB 등)로 라우팅
 
## PoFx - Power Management Framework
 
전원 관리자 섹션 후반부에서 처음 정의
- "The power management framework (PoFx) provides an API..."로 정의
- Windows 8에서 도입. 장치 내부의 개별 컴포넌트 단위 전원 상태 관리
- F 상태(F0=완전 동작~Fn=저전력) 관리 — D0 상태에서만 의미 있음
- Windows 10에서 성능 상태 관리(주파수·대역폭·커스텀) 기능 추가
- `PoFxRegisterDevice` → `PoFxActivateComponent` / `PoFxIdleComponent`로 제어
 
## PEP - Platform Extension Plug-in
 
PoFx 섹션에서 처음 등장
- "notify an OS service called platform extension plug-in (PEP)"로 정의
- 특정 프로세서 계열 또는 SoC에 특화된 전원 관리 플러그인
- Windows 8.x에서 드라이버가 성능 상태 변경을 PEP에 직접 통지해야 했음
- Windows 10 PoFx 확장으로 드라이버가 PEP에 직접 의존하지 않도록 추상화
 
## SoC - System on a Chip
 
PEP 설명에서 처음 등장
- "a particular line of processors or system on a chip (SoC)"로 정의
- CPU, GPU, 메모리 컨트롤러, I/O 인터페이스 등을 단일 칩에 통합한 설계
- 모바일 기기, IoT 장치에 주로 사용
- Windows 10 ARM 기반 디바이스(Surface Pro X 등)가 SoC 기반

---
 
# 7장 줄임말
 
## CC - Common Criteria
 
보안 등급 설명에서 처음 정의
- "the Common Criteria (CC)"로 등장
- 미국·유럽 등 다수 국가가 채택한 현행 국제 보안 평가 표준
- EAL(Evaluation Assurance Level) 1~7 등급으로 평가
- Windows는 CAPP 프로파일 하에 EAL 4+ 인증 획득
## TCSEC - Trusted Computer System Evaluation Criteria
 
CC 직후 보안 등급 섹션에서 처음 정의
- "the Trusted Computer System Evaluation Criteria (TCSEC)"로 정의
- 미국 국방부 오렌지북(Orange Book)으로 불림
- D(최저) ~ A1(최고) 등급 체계. Windows NT 설계 목표는 C2 등급 달성
- CC로 대체됐지만 Windows 보안 설계의 역사적 기반
## CAPP - Controlled Access Protection Profile
 
CC 인증 설명에서 처음 정의
- "Common Criteria (CC) certification under the Controlled Access Protection Profile (CAPP)"로 정의
- CC에서 정의한 보안 기능 프로파일 중 하나
- C2 등급과 대략 동등한 접근 제어 보호 요구사항을 명시
## EAL - Evaluation Assurance Level
 
CAPP 설명 직후 처음 등장
- "a rating of EAL 4+, the 'plus' denoting flaw remediation'"으로 등장
- CC의 보증 등급 체계 (EAL 1~7)
- EAL 4 : 국가 경계를 초월해 인정되는 가장 높은 등급
- EAL 4+ : EAL 4에 결함 수정 보증이 추가된 등급
## Lsass - Local Security Authority Subsystem Service
 
보안 시스템 구성 요소 섹션에서 처음 정의
- "Local Security Authority Subsystem Service (Lsass)"로 정의
- Lsass.exe로 실행되는 유저 모드 프로세스
- 로컬 보안 정책, 사용자 인증, 보안 감사 메시지 관리
- Lsasrv.dll이 대부분의 기능 구현. SAM·Active Directory·Kerberos 패키지를 로드
## KPP - Kernel Patch Protection
 
VBS 섹션에서 처음 정의
- "Kernel Patch Protection (KPP)"로 정의
- PatchGuard라고도 불림
- x64·ARM Windows에서 커널 코드·구조체 무단 수정을 감지해 BSOD로 대응
- 공격을 막는 것이 아닌 감지·충돌로 대응하는 메커니즘 (CCTV 비유)
## HVCI - HyperVisor Code Integrity
 
VBS 구성 요소 목록에서 처음 정의
- "Hypervisor-Based Code Integrity (HVCI)"로 정의
- Device Guard의 핵심 기술
- SLAT(하이퍼바이저 2차 주소 변환)를 통해 코드 서명 검증을 VTL 1에서 수행
- 커널이 침해되더라도 서명되지 않은 코드 로드·실행 불가
## KMCI - Kernel-Mode Code Integrity
 
HVCI 직후 첫 정의
- "Kernel-Mode Code Integrity (KMCI)"로 정의
- 커널 모드 드라이버 서명을 강제하는 코드 무결성 서비스
- HVCI가 KMCI를 VTL 1에서 강화
## NTOWF - NT One-Way Function
 
Credential Guard 섹션에서 처음 정의
- "NT one-way function (NT OWF)"로 정의
- 비밀번호의 MD4 해시. NTLM 프로토콜에서 사용자 인증에 활용
- 현대 시스템에서는 Kerberos로 대체됐으나 레거시 컴포넌트가 여전히 사용
- 인터셉트 시 즉시 사용 가능. MD4 취약성으로 비밀번호 복원도 가능
## MD4 - Message Digest 4
 
NTOWF 설명에서 처음 등장
- "NTOWF is an MD4 hash"로 등장
- Ron Rivest가 설계한 암호화 해시 알고리즘
- Windows NTLM 인증에서 NTOWF(비밀번호 해시) 생성에 사용
- 현재는 암호학적으로 취약해 보안 목적으로 부적합
## RDP - Remote Desktop Protocol
 
Credential Guard 섹션에서 처음 등장
- "Terminal Services/RDP"로 등장
- Microsoft의 원격 데스크톱 연결 프로토콜
- WDigest와 함께 평문 인증을 사용하는 프로토콜 예시로 언급
- Credential Guard가 이러한 프로토콜에 대한 SSO 기능을 비활성화
## UMCI - User-Mode Code Integrity
 
Device Guard 섹션에서 처음 정의
- "User-Mode Code Integrity (UMCI)"로 정의
- 사용자 모드 이미지(.exe, .dll)의 서명을 강제하는 코드 무결성 서비스
- HVCI가 SLAT를 통해 서명되지 않은 사용자 모드 페이지 실행도 차단
## CCI - Custom Code Integrity
 
Device Guard 섹션에서 처음 정의
- "Custom Code Integrity (CCI)"로 정의
- 기업 관리자가 직접 정의하는 코드 서명 정책
- Secure Boot로 보호되며 인증서 서명자나 SHA-2 해시 기반 포함·제외 목록 정의
## SHA - Secure Hash Algorithm
 
Device Guard 정책 설명에서 처음 등장
- "certificate signers or SHA-2 hashes"로 등장
- 미국 NIST가 표준화한 암호화 해시 알고리즘 계열
- SHA-1(160비트, 현재 취약), SHA-2(SHA-256/384/512), SHA-3 포함
- AppLocker·Device Guard·코드 서명 등 Windows 보안 전반에서 파일 무결성 확인에 활용
## MBEC - Mode-Based Execution Control
 
Device Guard 섹션에서 처음 정의
- "Mode-Based Execution Control (MBEC)"로 정의
- SLAT 페이지 테이블 엔트리에 유저 모드·커널 모드 별도 실행 비트를 추가하는 하드웨어 기능
- MBEC가 없는 하드웨어에서는 Restricted User Mode(RUM)로 소프트웨어 에뮬레이션
## RUM - Restricted User Mode
 
MBEC 설명 직후 처음 정의
- "Restricted User Mode (RUM)"로 정의
- MBEC를 지원하지 않는 하이퍼바이저가 MBEC 기능을 소프트웨어로 에뮬레이션하는 방식
- Device Guard의 유저/커널 실행 모드 분리를 MBEC 없이 구현
## SKCI - Secure Kernel Code Integrity
 
Device Guard 마지막 부분에서 처음 정의
- "a library called SKCI.DLL, or Secure Kernel Code Integrity"로 정의
- VTL 1(보안 커널)에 위치한 코드 무결성 라이브러리
- Device Guard의 모든 코드 서명 강제를 VTL 1에서 최종 수행
## DACL - Discretionary Access Control List
 
오브젝트 보호 섹션에서 처음 정의
- "object's discretionary access control list (DACL)"로 정의
- 보안 서술자의 핵심 구성 요소. 오브젝트 접근 허용·거부 ACE 목록
- NULL DACL = 모두 허용 (AppContainer에서는 예외적으로 거부 처리)
- 빈(Empty) DACL = 모두 거부
## UIPI - User Interface Privilege Isolation
 
무결성 레벨 섹션에서 처음 정의
- "User Interface Privilege Isolation (UIPI)"로 정의
- 낮은 무결성 프로세스가 높은 무결성 프로세스의 윈도우에 메시지를 보내거나 훅을 설치하는 것을 차단
- UAC와 함께 도입된 UI 격리 메커니즘. AppContainer에도 적용
## RID - Relative Identifier
 
SID 섹션에서 처음 정의
- "32-bit subauthority or relative identifier (RID) values"로 정의
- SID의 마지막 부분. 도메인·컴퓨터 내에서 계정을 고유하게 식별하는 값
- Administrator = 500, Guest = 501, 일반 계정 = 1000부터 증가
- 같은 도메인 SID + 다른 RID = 다른 계정
## ACE - Access Control Entry
 
SID 섹션에서 처음 정의
- "access control entry (ACE)"로 정의
- DACL 또는 SACL을 구성하는 개별 항목
- 구성 : 유형(허용/거부/감사) + SID + 접근 마스크
- 거부 ACE가 허용 ACE보다 항상 우선 적용
## MMC - Microsoft Management Console
 
서비스 관련 실험에서 처음 등장
- "Services MMC snap-in (services.msc)"로 등장
- Windows 관리 도구를 통합하는 호스트 애플리케이션 프레임워크
- 스냅인(snap-in) 형태로 다양한 관리 도구 추가 가능 (보안 정책, 장치 관리자 등)
- secpol.msc(로컬 보안 정책), services.msc, compmgmt.msc 등이 MMC 스냅인
## SACL - System Access Control List
 
보안 서술자 섹션에서 처음 정의
- "System access control list (SACL)"로 정의
- 보안 서술자의 구성 요소. 어떤 사용자의 어떤 작업을 보안 감사 로그에 기록할지 정의
- SACL을 설정·조회하려면 SeSecurityPrivilege 필요
- 전역 감사 정책(Global Audit Policy)으로 모든 파일/레지스트리에 일괄 적용 가능
## SDDL - Security Descriptor Definition Language
 
보안 서술자 섹션에서 처음 정의
- "Security Descriptor Definition Language (SDDL)"로 정의
- 보안 서술자를 컴팩트한 문자열로 표현하는 언어
- AppLocker 규칙, 오브젝트 보안 설정 등에 사용
- 예 : `D:(XD;;FX;;;SID;(APPID://FQBN >= {..."}))`
## UAP - Universal Application Platform
 
AppContainers 섹션에서 처음 정의
- "Universal Application Platform (UAP) is sometimes used instead of UWP"로 정의
- UWP(Universal Windows Platform)의 이전 명칭
- Windows 8에서 처음 도입된 앱 플랫폼으로 다양한 Windows 10 폼팩터에서 실행
## OID - Object Identifier
 
AppContainer capability 섹션에서 처음 정의
- "a mapping between an object identifier (OID)"로 정의
- 인증서에서 특정 오브젝트 유형(예: EKU)을 고유하게 식별하는 숫자 문자열
- AppContainer capability SID와 인증서 OID를 매핑하는 레지스트리 키에서 사용
## CRT - C Runtime (Universal CRT)
 
Device Guard 동적 코드 섹션에서 처음 등장
- "the Universal CRT (uCRT) itself"로 등장
- C언어 런타임 라이브러리. Windows 10부터 uCRT(Universal CRT)로 통합
- Device Guard 컨텍스트에서 NGEN.EXE와 함께 동적 코드 생성이 허용되는 예시로 언급
## DCOM - Distributed Component Object Model
 
AppLocker AppID 설명에서 처음 등장
- "not the same as the AppID used by DCOM/COM+ applications"로 등장
- COM을 네트워크상의 원격 오브젝트 접근을 지원하도록 확장한 기술
- AppLocker의 AppID(Application Identity)와 DCOM의 AppID는 다른 개념임을 명시하는 맥락
## FQBN - Fully Qualified Binary Name
 
AppLocker 규칙 기준 설명에서 처음 정의
- "APPID://FQBN is a fully qualified binary name"으로 정의
- 서명된 파일의 전체 이진 이름 : `{게시자\제품명\파일명, 버전}` 형식
- AppLocker의 서명 인증서 기반 규칙에서 파일을 식별하는 핵심 속성
## SRP - Software Restriction Policies
 
AppLocker 섹션에서 처음 등장
- "Windows XP introduced Software Restriction Policies (SRP)"로 정의
- Windows XP에서 도입된 초기 코드 실행 제어 메커니즘
- AppLocker로 대체됐지만 공존 (같은 GPO에서는 AppLocker 규칙 우선)
- 단점 : 사용자·그룹별 적용 불가, 감사 모드 없음
## GPO - Group Policy Object
 
AppLocker 섹션에서 처음 정의
- "the same Group Policy object (GPO)"로 정의
- Active Directory 도메인에서 컴퓨터·사용자 설정을 관리하는 정책 컨테이너
- AppLocker 규칙을 도메인 전체에 배포할 때 활용
- secpol.msc(로컬)나 gpmc.msc(도메인)로 편집
## OCX - OLE Control Extension
 
AppLocker 제어 파일 목록에서 처음 등장
- "Dynamic-link libraries (DLL and OCX)"로 등장
- ActiveX 컨트롤의 파일 확장자. COM 기반 재사용 가능 UI 컴포넌트
- AppLocker가 DLL과 함께 제어 대상으로 지정
## MSI - Microsoft Software Installer (Windows Installer Package)
 
AppLocker 제어 파일 목록에서 처음 정의
- "Microsoft Software Installer (MSI and MSP)"로 등장
- Windows 설치 패키지 파일 형식. 설치·제거·업데이트 기능 포함
- AppLocker가 설치 및 제거 동작 모두 제어 가능
## PS1 - PowerShell Script
 
AppLocker 스크립트 목록에서 처음 등장
- "Windows PowerShell (PS1)"로 등장
- PowerShell 스크립트 파일 확장자
- AppLocker가 BAT, CMD, VBS, JS와 함께 스크립트 실행 제어 대상으로 포함
## BAT - Batch Script
 
AppLocker 스크립트 목록에서 처음 등장
- "Batch (BAT and CMD)"로 등장
- Windows 배치 스크립트 파일 확장자
- AppLocker의 스크립트 제어 대상
## CMD - Command Script
 
BAT와 함께 처음 등장
- "Batch (BAT and CMD)"로 등장
- Windows 명령 스크립트 파일 확장자 (.cmd)
- BAT와 동일한 배치 스크립트이나 구분하여 관리
## CCTV - Closed-Circuit Television
 
PatchGuard 비유 설명에서 처음 등장
- "Internet-connected video security system, or CCTV"로 등장
- PatchGuard의 역할을 비유하는 데 사용 : 공격을 막는 잠금장치가 아닌 감지·경보 시스템
- KPP는 CCTV처럼 사후 감지 후 알람(BSOD)을 울릴 뿐 공격 자체를 차단하지 않음
## SSDT - System Service Descriptor Table
 
PatchGuard 보호 대상 표에서 처음 정의
- "System Service Descriptor Table (SSDT)"로 정의
- 각 시스템 콜 핸들러를 가리키는 함수 포인터 배열 테이블
- 루트킷이 가장 많이 노리는 커널 구조체 중 하나
- PatchGuard가 무단 수정 감지·BSOD로 보호
## LWF - Lightweight Filter
 
PatchGuard 지원 대체 메커니즘 섹션에서 처음 정의
- "NDIS Lightweight Filters (LWF)"로 정의
- 네트워크 드라이버 인터페이스(NDIS) 스택에 삽입되는 필터
- 소켓 연산 감시, 원시 이더넷 프레임 데이터 접근 가능
- PatchGuard 대신 활용 가능한 지원 커널 확장 메커니즘 중 하나
## WFP - Windows Filtering Platform
 
LWF와 함께 처음 정의
- "Windows Filtering Platform (WFP) filters"로 정의
- 네트워크 패킷 필터링을 위한 Windows 표준 플랫폼
- accept, listen, connect, close 등 소켓 연산과 패킷 자체 감시 가능
- 방화벽·IDS 등 보안 소프트웨어가 활용
## NIC - Network Interface Card
 
WFP 설명에서 처음 등장
- "data that is going from the network card (NIC) to the wire"로 등장
- 네트워크 인터페이스 카드. 컴퓨터를 네트워크에 연결하는 하드웨어
- LWF를 통해 NIC에서 전선으로 나가는 원시 이더넷 프레임 데이터에 접근 가능
## NPIEP - Non-Privileged Instruction Execution Prevention
 
HyperGuard 섹션에서 처음 정의
- "Non-Privileged Instruction Execution Prevention (NPIEP)"로 정의
- VBS 활성화 시 하이퍼바이저가 구현하는 완화 기법
- SGDT/SIDT/SLDT 명령 실행은 허용하되 실제 커널 주소 대신 프로세서별 고유 가상 값 반환
- KASLR(커널 ASLR) 정보 유출 공격 방어
## LDT - Local Descriptor Table
 
NPIEP 섹션에서 처음 등장
- "kernel-mode addresses of the GDT, IDT, and LDT"로 등장
- CPU의 세그먼트 서술자 테이블 중 하나 (GDT와 별도로 프로세스별 세그먼트 정의)
- SLDT 명령으로 주소를 읽을 수 있어 KASLR 우회에 악용 가능 → NPIEP로 보호
## SGDT / SIDT / SLDT - Store GDT/IDT/LDT Register
 
NPIEP 섹션에서 처음 등장
- "which are SGDT, SIDT, and SLDT"로 등장
- x64 명령어로 각각 GDT, IDT, LDT의 커널 주소를 읽는 명령
- 정보 유출(KASLR 우회)에 악용 가능
- NPIEP 적용 시 실제 주소 대신 프로세서별 고유 가상 값 반환
## KASLR - Kernel Address Space Layout Randomization
 
HyperGuard 마지막 부분에서 처음 정의
- "Kernel ASLR (KASLR)"로 정의
- 부팅 시 커널 이미지·드라이버·커널 구조체의 메모리 주소를 무작위화
- 커널 주소를 알아야 실행 가능한 공격을 어렵게 만드는 완화 기법
- NPIEP는 KASLR 우회 가능한 정보 유출 명령으로부터 보호
