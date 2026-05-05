# 1장 줄임말

## OS - Operating System

- 운영체제. 챕터 도입부 첫 문장에서 "Windows operating system (OS)"로 등장. 이하 모든 논의의 대상

## API - Application Programming Interface

- 응용 프로그램과 OS 사이의 약속된 함수 집합
- 윈도우 API는 사용자 모드에서 OS 서비스를 호출하는 인터페이스
- CreateProcess, CreateFile 같은 C 스타일 함수 수천 개로 구성
- 32비트판은 Win32 API라고도 부름

## WDK - Windows Driver Kit

- 드라이버 개발 도구 패키지
- ntddk.h, wdm.h 등 핵심 헤더와 커널 지원 함수 문서 포함
- 체크드 빌드 커널·HAL도 여기서 제공

## SDK - Software Development Kit

- Windows 애플리케이션 개발 도구 패키지
- Windows API 헤더·라이브러리 포함
- Debugging Tools for Windows(cdb, ntsd, kd, WinDbg) 포함

## MSDN - Microsoft Developer Network

- 마이크로소프트 개발자 지원 프로그램
- Windows API 문서, SDK/WDK 다운로드, 코드 샘플 제공
- 현재는 docs.microsoft.com으로 통합

## COM - Component Object Model

- C 스타일 API의 명명 비일관성 문제를 해결하려 만든 인터페이스 기반 컴포넌트 모델
- 원래 Office 문서 간 데이터 교환(OLE)을 위해 탄생
- COM 1. 클라이언트는 vtable 인터페이스를 통해 객체와 통신 (C, C++, VB, .NET 등 언어 무관)
- COM 2. 구현체(DLL/EXE)는 런타임에 동적 로드
- DirectX, WIC, BITS 등이 COM으로 노출
- 추천 도서 Essential COM(Don Box, Addison-Wesley, 1998)

## OLE - Object Linking and Embedding

- 엑셀 차트를 워드 문서에 삽입하는 등 문서 간 객체 연결·삽입 기술
- COM의 직접적 전신
- 초기에는 DDE로 구현됐다가 COM 기반(OLE 2)으로 재설계됨

## DDE - Dynamic Data Exchange

- Windows 메시지 기반의 구형 프로세스 간 데이터 교환 메커니즘
- OLE 초기 구현에 사용됐으나 본질적 한계로 COM으로 대체됨

## DLL - Dynamic Link Library

- 런타임에 동적으로 로드되는 공유 라이브러리
- 여러 프로세스가 단일 인메모리 복사본을 공유
- COM 서버 = *.DLL or *.exe
- Kernel32.dll, Ntdll.dll, Msvcrt.dll 등이 대표 예시

## WinRT - Windows Runtime

- Windows 8에서 도입된 UWP(스토어 앱) 전용 API 및 런타임
- COM을 확장한 구조, 타입 메타데이터를 WINMD 파일에 저장
- 네임스페이스 계층, 일관된 명명, 프로그래밍 패턴으로 Win32보다 정교
- C++/CX, C#, JavaScript(WinJS)에서 소비 가능
- ※ Windows RT(ARM 전용 OS)와 혼동 주의

## CLR - Common Language Runtime

- .NET 프레임워크의 실행 엔진
- JIT 컴파일러, 가비지 컬렉터, 타입 검증, 코드 접근 보안 등 제공
- COM 인프로세스 서버(DLL)로 구현됐으며 Windows API를 내부적으로 활용

## JIT - Just-In-Time

- CLR이 CIL(중간 언어) 바이트코드를 실행 시점에 네이티브 CPU 기계어로 컴파일하는 방식

## CIL - Common Intermediate Language

- .NET 컴파일러(C#, VB, F# 등)가 생성하는 플랫폼 독립 중간 언어
- CLR의 JIT 컴파일러가 최종 CPU 명령어로 변환

## FCL - .NET Framework Class Library

- .NET이 제공하는 대규모 타입 라이브러리
- UI, 네트워킹, 데이터베이스 접근 등 클라이언트·서버 앱에 필요한 기능 제공

## UAC - User Account Control

- 관리자 그룹 사용자 로그인 시 표준 권한 토큰 + 관리자 권한 토큰 두 개 발급
- 필요 시에만 상승 권한을 사용하게 하는 보안 메커니즘

## WMI - Windows Management Instrumentation

- 드라이버·서비스가 성능·구성 정보를 게시하고 관리 명령을 수신하는 표준 프레임워크
- CIM/MOF 언어 기반
- PowerShell의 Get-WmiObject 등으로 조회

## TLS - Thread-Local Storage

- 스레드마다 독립적으로 유지되는 개인 저장 영역
- 서브시스템, 런타임 라이브러리, DLL에서 스레드 전용 데이터를 보관
- 파이버는 TLS를 공유하는 문제가 있어 FLS(Fiber-Local Storage)가 별도 존재

## UMS - User-Mode Scheduling

- 64비트 Windows 전용 경량 스레드 스케줄링
- 파이버의 장점을 제공하면서 커널에도 스레드 상태가 보임
- 블로킹 시스템 콜도 독립적으로 처리 가능
- 사용자 모드에서 컨텍스트 스위치 수행 → 커널 스케줄러 호출 감소

## AWE - Address Windowing Extensions

- 32비트 프로세스의 2GB 가상 공간 한계를 극복하는 API 메커니즘
- 최대 64GB 물리 메모리를 매핑·사용 가능
- 대용량 데이터베이스 서버에서 메모리 뷰를 직접 전환하는 방식으로 활용

## DEP - Data Execution Prevention

- 데이터 페이지를 non-executable로 표시해 데이터 영역에서의 코드 실행을 막는 하드웨어 기반 보호 기능
- CPU의 NX(No-Execute) / XD(Execute Disable) 비트 활용

## KMCS - Kernel-Mode Code Signing

- 64비트·ARM Windows 8.1 이상에서 모든 드라이버에 암호화 서명을 요구하는 정책
- Windows 10은 EV 인증서 + Microsoft 증명 서명(Attestation Signing)까지 요구
- Server 2016은 WHQL 인증까지 필요

## EV - Extended Validation

- KMCS 정책 강화 맥락에서 등장
- SHA-2 기반 EV 하드웨어 인증서
- Windows 10에서 새 드라이버가 반드시 취득해야 하는 인증서 유형
- 일반 파일 기반 SHA-1 인증서를 대체

## WHQL - Windows Hardware Quality Labs

- 마이크로소프트의 공식 드라이버 인증 프로그램
- HCK(Hardware Compatibility Kit)로 호환성·보안·성능·안정성 검증
- Server 2016은 WHQL 서명 드라이버만 로드 허용

## VBS - Virtualization-Based Security

- Hyper-V 하이퍼바이저를 이용해 VTL 1 격리 실행 환경을 만들고 구현하는 보안 기술 집합
- 포함 기술 : Device Guard(HVCI), Hyper Guard, Credential Guard, Application Guard, Host Guardian/Shielded Fabric

## TPM - Trusted Platform Module

- 시스템 무결성 측정·증명(attestation)을 위한 하드웨어 칩
- UEFI Secure Boot, BitLocker, Measured Boot 등에 사용
- v-TPM은 가상 머신용 소프트웨어 TPM

## VTL - Virtual Trust Level

- 하이퍼바이저가 제공하는 직교적 격리 계층
- VTL 0 = 일반 커널, VTL 1 = 보안 커널
- 권한 레벨(ring 0/3)과 독립적으로 동작
- VTL 0 커널조차 VTL 1 메모리를 조작할 수 없음

## UEFI - Unified Extensible Firmware Interface

- BIOS를 대체하는 현대 펌웨어 인터페이스
- Secure Boot 필수 요구, 부트 소프트웨어의 서명 검증
- Windows 8 이상 인증 시스템에서 신뢰 체인의 시작점

## GUI - Graphical User Interface

- 그래픽 사용자 인터페이스
- Terminal Services 설명에서 "서버가 GUI를 원격 클라이언트에 전송"으로 등장

## CCITSE - Common Criteria for Information Technology Security Evaluation

- 정부·산업계의 OS 보안 등급 평가 기준
- Windows는 이 스펙의 요구사항을 충족하도록 설계됨
- 정부 조달 요건을 만족시킴

## BSTR - Basic String

- COM 기반 API가 사용하는 유니코드 문자열 타입
- null-terminated 유니코드 배열
- 배열 시작 4바이트 앞에 문자열 길이(바이트)가 저장됨

## HSTRING - Handle String

- Windows Runtime API가 사용하는 불변(immutable) 유니코드 문자열 타입
- WinRT는 유니코드 문자열만 사용

## WinDbg - Windows Debugger

- GUI 기반 커널·사용자 모드 겸용 디버거
- DbgEng.dll 엔진 기반
- 커널 디버깅 시 !process, dt, !thread 등 확장 명령으로 내부 구조 탐색 가능

---

# 2장 줄임말

## NT - New Technology

- 1989년 설계 요구사항 목록과 함께 소개
- Windows의 내부 커널 이름이자 제품 계보의 시작점
- 현재도 Ntoskrnl.exe, Ntdll.dll 등으로 NT 네이밍 유지

## POSIX - Portable Operating System Interface

- IEEE 1003.1 UNIX 호환 인터페이스 표준
- NT는 POSIX 서브시스템으로 이를 지원했으나 Windows 8에서 폐지
- SUA(Subsystem for UNIX-based Applications)가 마지막 구현체

## SUA - Subsystem for UNIX-based Applications

- Windows 7 Ultimate/Enterprise 및 Server 2008 R2에서 제공된 강화 POSIX 서브시스템
- Windows 8에서 폐지

## WSL - Windows Subsystem for Linux

- Pico 프로바이더(Lxss.sys + LxCore.sys) 기반 Linux 바이너리 실행 환경
- PE 재컴파일 없이 ELF 실행 파일을 구동
- Windows 10 1607에서 개발자 베타로 도입

## HAL - Hardware Abstraction Layer

- Hal.dll
- 커널과 드라이버가 하드웨어를 직접 다루지 않고 HAL 루틴을 통해 접근하도록 추상화
- 인터럽트 컨트롤러, I/O 인터페이스, 멀티프로세서 통신 메커니즘 차이를 감춤

## GDI - Graphics Device Interface

- Win32k.sys에 포함된 2D 그래픽 출력 라이브러리
- 애플리케이션이 하드웨어 세부사항 없이 디스플레이·프린터를 사용하도록 표준 인터페이스 제공

## SMP - Symmetric Multiprocessing

- 모든 CPU가 동일한 메모리 공간을 공유하고 어떤 CPU에서도 OS 코드·스레드가 실행될 수 있는 구조
- ASMP(비대칭 멀티프로세싱)와 대비됨

## ASMP - Asymmetric Multiprocessing

- OS 커널 코드가 특정 CPU 하나에서만 실행되고 나머지는 사용자 코드만 실행하는 방식
- Windows는 ASMP를 사용하지 않음

## SMT - Simultaneous Multithreading

- 단일 물리 코어가 두 개 이상의 논리 프로세서를 제공하는 기술
- Intel 하이퍼-스레딩, AMD Zen SMT가 대표적
- Windows 스케줄러는 물리 코어와 논리 프로세서를 구분 관리

## NUMA - Non-Uniform Memory Access

- CPU를 노드로 묶고 노드 로컬 메모리 접근이 더 빠른 아키텍처
- Windows는 SMP로 동작하면서 노드 친화도 스케줄링으로 성능 최적화

## ETW - Event Tracing for Windows

- 두 메커니즘 모두 ETW 위에 구축됨
- WPA(Windows Performance Analyzer)로 분석

## VSM - Virtual Secure Mode

- VBS와 동의어
- "You'll also sometimes see the term Virtual Secure Mode, or VSM, used."
- Windows 10 1607·Server 2016부터 지원 하드웨어에서 기본 활성화

## IUM - Isolated User Mode

- VTL 1에서 실행되는 사용자 모드 환경
- 허용된 시스템 콜만 사용 가능 (I/O·그래픽·드라이버 통신 불가)
- Iumdll.dll(VTL 1판 Ntdll.dll), Iumbase.dll을 통해 노출
- Trustlet만 IUM에서 실행 가능

## SLAT - Second Level Address Translation

- 하이퍼바이저가 게스트 물리 주소(GPA)를 실제 물리 주소(SPA)로 변환하는 하드웨어 기능
- Intel = EPT, AMD = NPT, ARM = Stage-2

## DMA - Direct Memory Access

- CPU를 거치지 않고 디바이스가 직접 메모리에 접근하는 전송 방식
- IOMMU로 DMA 범위를 제한해 드라이버가 하이퍼바이저·보안 커널 물리 메모리에 접근하는 것을 차단

## IOMMU - I/O Memory Management Unit

- 디바이스의 DMA를 가상화·제한하는 하드웨어 유닛
- VBS에서 드라이버가 SLAT를 우회해 하이퍼바이저·보안 커널 메모리에 접근하는 것을 막는 핵심 장치

## NTFS - New Technology File System

- Windows 기본 파일 시스템
- 복구 가능성(트랜잭션 로그), 보안, EFS 암호화, 압축, 변경 저널 등 고급 기능 지원

## ALPC - Advanced Local Procedure Call

- 동일 시스템 내 프로세스 간 메시지 전달 메커니즘
- RPC의 로컬 전송으로 사용

## PE - Portable Executable

- Windows 실행 파일·DLL 포맷
- 이미지 헤더에 서브시스템 타입, 임포트/익스포트 테이블 등을 기록

## APC - Asynchronous Procedure Call

- 특정 스레드에 비동기적으로 함수를 주입·실행하는 메커니즘
- 커널 APC와 유저 APC(ReadFileEx 완료 루틴 등)로 구분

## SRM - Security Reference Monitor

- 커널 내부의 보안 정책 집행기
- 객체 접근 시 호출자의 토큰과 ACL을 비교해 허용·거부를 결정하고 감사 이벤트를 기록

## PnP - Plug and Play

- 디바이스 추가·제거를 자동으로 감지하고 드라이버를 로드·언로드하는 메커니즘
- 하드웨어 리소스(IRQ, I/O 포트, DMA, 메모리)를 각 디바이스에 할당

## WDM - Windows Driver Model

- Windows 2000에서 도입된 드라이버 모델
- PnP·전원 관리 지원
- Bus·Function·Filter 드라이버의 3계층 구조

## LPC - Local Procedure Call

- ALPC의 전신이자 기반 개념
- 동일 머신 내 프로세스 간 통신

## RPC - Remote Procedure Call

- 네트워크를 통해 원격 서버 함수를 호출하는 표준 프로토콜
- ALPC가 로컬 RPC 전송을 담당

## DPC - Deferred Procedure Call

- 인터럽트 서비스 루틴이 우선도가 낮은 후속 처리를 지연 실행하는 메커니즘
- DISPATCH_LEVEL(IRQL 2)에서 실행
- KPRCB에 DPC 큐가 포함됨

## KPCR - Kernel Processor Control Region

- CPU별 전용 커널 데이터 구조
- IDT, TSS, GDT, 인터럽트 컨트롤러 상태 포함
- x32는 fs 레지스터, x64는 gs 레지스터를 통해 빠르게 접근
- !pcr 디버거 명령으로 조회

## IDT - Interrupt Descriptor Table

- x86/x64 인터럽트 벡터 테이블
- 각 항목(게이트)이 하드웨어 인터럽트·예외 핸들러를 가리킴
- !idt 디버거 명령으로 조회 가능

## TSS - Task State Segment

- x86/x64 하드웨어 태스크 상태 세그먼트
- 커널 스택 포인터, IST(Interrupt Stack Table) 항목을 보관
- 인터럽트 처리 시 스택 전환에 사용

## GDT - Global Descriptor Table

- x86/x64 세그먼트 디스크립터 배열
- CS·DS·SS·FS·GS 등 세그먼트 기저 주소·한계·특권 레벨을 정의

## KPRCB - Kernel Processor Control Block

- KPCR 안에 내장된 사적(private) 구조체
- 현재·다음·유휴 스레드 스케줄링 정보, DPC 큐, CPU 토폴로지·캐시 크기·시간 통계 보관
- WDK에 문서화 안 됨
- !prcb 명령으로 조회

## IRQL - Interrupt Request Level

- Windows 커널의 인터럽트 우선순위 레벨 (PASSIVE=0 ~ HIGH=31)
- 높은 IRQL은 낮은 IRQL 실행을 선점 가능
- 스핀락·DPC 처리 등과 긴밀히 연결됨

## APIC - Advanced Programmable Interrupt Controller

- 현대 인터럽트 라우팅을 담당하는 컨트롤러
- 구형 PIC를 대체

## WDF - Windows Driver Foundation

- WDM의 복잡도를 숨기는 프레임워크
- KMDF(커널 모드)와 UMDF(사용자 모드)로 구성
- GitHub 오픈소스 공개

## KMDF - Kernel-Mode Driver Framework

- WDM 위에서 동작하며 전원 관리·동기화 등 반복 코드를 줄여줌
- Windows 2000 SP4 이상 지원

## UMDF - User-Mode Driver Framework

- USB·고지연 프로토콜 디바이스(카메라, MP3 플레이어, 프린터 등)에 적합
- 드라이버 충돌 시 프로세스 재시작으로 복구 가능
- UMDF 2.0부터 KMDF와 동일한 객체 모델 공유

## SAM - Security Account Manager

- 로컬 사용자·그룹 정보를 저장하는 레지스트리 하이브(HKLM\SAM)
- Lsass.exe가 로컬 인증 시 조회

## LSA - Local Security Authority

- 인증 정책 시행·로그온 처리를 담당하는 보안 서브시스템 서비스(Lsass.exe)
- 인증 패키지 DLL을 통해 암호 확인, 토큰 생성 수행

## SCM - Service Control Manager

- Services.exe 프로세스
- 서비스 시작·중지·재시작 및 상태 보고를 처리
- HKLM\SYSTEM\CurrentControlSet\Services 레지스트리 기반 동작

## SAS - Secure Attention Sequence

- Ctrl+Alt+Delete 키 조합
- 사용자 모드 애플리케이션이 가로챌 수 없어 로그인 화면 스푸핑을 방지
- SAS 감지 시 Winlogon이 LogonUI.exe를 실행해 자격 증명 공급자를 초기화

---

# 3장 줄임말

## RPC - Remote Procedure Call

- 네트워크를 통해 원격 서버 함수를 호출하는 표준 프로토콜

## PE - Portable Executable

- Windows 실행 파일(.exe, .dll) 포맷의 공식 명칭

## EXE - Executable File

- Windows 실행 파일 확장자

## UWP - Universal Windows Platform

- CreateProcess만으로 생성 불가 → IApplicationActivationManager COM 인터페이스 필요
- 모든 UWP 앱은 잡(Job) 안에서 실행됨

## EPROCESS - Executive Process Structure

- Windows가 각 프로세스를 표현하는 커널 내부 핵심 데이터 구조
- 첫 번째 멤버 = Pcb (KPROCESS)
- KTHREAD, PEB, VAD 등을 포함 또는 포인팅

## ETHREAD - Executive Thread Structure

- 각 스레드를 표현하는 커널 내부 핵심 데이터 구조
- 첫 번째 멤버 = Tcb (KTHREAD)
- 시스템 주소 공간에 존재 (TEB 제외)

## PEB - Process Environment Block

- EPROCESS 관련 데이터 구조 중 유일하게 프로세스(유저) 주소 공간에 존재하는 구조체
- 이미지 로더, DLL 목록, 환경 변수 등 유저모드 코드가 접근하는 정보 포함

## KPROCESS - Kernel Process Structure

- EPROCESS의 첫 번째 멤버인 Pcb(Process Control Block)의 실제 타입으로 정의
- 디스패처·스케줄러·인터럽트·시간 계산 코드가 EPROCESS 대신 KPROCESS를 사용 → 계층 분리

## CSR - Client/Server Runtime (Subsystem)

- Windows 서브시스템 프로세스(Csrss.exe)가 각 Windows 프로그램 프로세스에 대해 병렬로 유지하는 구조체의 접두사

## PMP - Protected Media Path

- Blu-ray 등 고품질 디지털 콘텐츠의 안전한 재생을 위한 Windows 구성 요소
- Media Foundation API를 통해 보호 프로세스를 활용

## WER - Windows Error Reporting

- 보호 프로세스 크래시 시 접근이 필요하기 때문

## PPL - Protected Process Light

- 보호 프로세스의 확장 모델
- Signer 값에 따라 보호 수준이 차등화됨
- smss.exe, csrss.exe, services.exe, wininit.exe 등이 WinTcb-Lite로 실행

| Signer | 수준 | 사용 예 |
|--------|------|---------|
| WinSystem | 최고 | System 프로세스, Memory Compression |
| WinTcb | 높음 | smss.exe, csrss.exe, services.exe, wininit.exe |
| Windows | 중간 | sppsvc.exe |
| Antimalware | 중간 | MsMpEng.exe (Windows Defender) |

## DRM - Digital Rights Management

- 멀티미디어 DRM → Windows 라이선싱 DRM → Windows Store DRM으로 확장

## AM - Anti-Malware

- Code Integrity 모듈의 EKU OID 검증으로 이를 방지

## EKU - Enhanced Key Usage

- 디지털 코드 서명 인증서에 인코딩되는 특수 OID 필드
- PPL 사이너 결정에 사용
- 예: Windows System Component Verification EKU(1.3.6.1.4.1.311.10.3.6)가 있어야 WinSystem 사이너 획득 가능

## PID - Process Identifier

- 시스템 내 프로세스의 고유 식별 번호

## ELAM - Early-Launch Anti-Malware

- 부팅 초기에 로드되는 안티멀웨어 드라이버
- Microsoft 발급 특수 인증서 필요
- ELAMCERTIFICATEINFO PE 리소스 섹션을 통해 AM 서비스가 PPL 수준 획득

## OID - Object Identifier

- ASN.1 기반의 전역 고유 식별자 번호 체계
- 디지털 인증서의 EKU를 식별하는 데 사용

## TEB - Thread Environment Block

- 스레드 전용 유저모드 데이터 구조
- PEB와 마찬가지로 프로세스(유저) 주소 공간에 존재

## KTHREAD - Kernel Thread Structure

- ETHREAD의 첫 번째 멤버(Tcb)
- 스케줄링·동기화·시간 계산 데이터를 보관하는 커널 스레드 구조체

## ETW - Event Tracing for Windows

- 커널·유저모드 컴포넌트의 시스템 전체 이벤트 추적 인프라
- WPP 트레이싱, TraceLogging이 ETW 위에 구축됨

## SVN - Security Version Number

- 트러스틀릿이 서명·암호화 데이터의 출처를 강력히 증명할 때 사용하는 버전 번호
- Credential Guard, Key Guard의 AES256/GCM 암호화에 활용

## SDF - Secure Device Framework

트러스틀릿이 보안 하드웨어 디바이스를 유저모드 드라이버로 다루기 위한 프레임워크
- SDFHost.dll에 포함
- Secure Biometrics(Windows Hello용 지문 센서, 웹캠 등)에 활용

## FIPS - Federal Information Processing Standard

- 미국 연방 정보 처리 표준
- Secure Kernel의 FIPS 모드 여부를 트러스틀릿이 조회 가능

## RNG - Random Number Generator

- Secure Kernel이 IUM 전용으로 생성하는 난수 시드
- 유저모드에서는 접근 불가

## SEH - Structured Exception Handling

- Windows의 구조화 예외 처리 메커니즘
- 트러스틀릿도 Exception and Continue API를 통해 SEH 지원

## VDM - Virtual DOS Machine

- 16비트 MS-DOS 애플리케이션 실행을 위한 가상 환경

## TID - Thread Identifier

- 프로세스 생성 결과로 반환되는 초기 스레드의 고유 ID

## NX - No-Execute

- CPU 하드웨어 기능으로 데이터 페이지를 실행 불가로 표시
- DEP의 하드웨어 구현체
- AMD는 NX, Intel은 XD(Execute Disable)라 부름

## IFEO - Image File Execution Options

- HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options
- 실행 파일별 디버거 지정, 힙 디버그 옵션, 전역 플래그 등 설정

## VAD - Virtual Address Descriptor

- 메모리 관리자가 프로세스의 가상 주소 사용 현황을 추적하는 트리 자료구조
- VirtualAlloc 등으로 예약된 범위가 VAD로 관리됨

## NLS - National Language Support

- 다국어 지원을 위한 문자 코드 변환·정렬·로케일 처리 테이블

## IRQL - Interrupt Request Level

- 인터럽트 우선순위 레벨 (PASSIVE=0 ~ HIGH=31)
- 새 스레드가 DPC 레벨에서 APC 레벨로 낮아진 후 PspUserThreadStartup 호출

## DPC - Deferred Procedure Call

- 인터럽트 서비스 루틴이 후속 처리를 지연 실행하는 메커니즘
- DISPATCH_LEVEL(IRQL 2)에서 실행

## APC - Asynchronous Procedure Call

- 특정 스레드에 비동기적으로 함수를 주입·실행하는 메커니즘
- 커널 APC(시스템 전용)와 유저 APC(ReadFileEx 완료 루틴 등)로 구분

## FLS - Fiber-Local Storage

- 파이버(Fiber) 전용 개인 저장 영역
- TLS와 별개로 존재

## SxS - Side-by-Side

- Windows Side-by-Side 어셈블리 기술
- 같은 DLL의 여러 버전이 동시에 공존할 수 있게 하는 매니페스트 기반 DLL 격리 메커니즘
- DLL Hell 해결

## MUI - Multilingual User Interface

- 다국어 UI 리소스 파일
- 단일 바이너리가 여러 언어 UI를 지원할 수 있게 해주는 Windows 국제화 메커니즘

## IAT - Import Address Table

- PE 실행 파일이 사용하는 외부 DLL 함수 주소 테이블
- 로더가 로드 시 실제 함수 주소로 채움

## MRDATA - Mutable Read-Only Data

- 보안 관련 전역 변수를 저장하는 특수 힙 섹션
- 익스플로잇에 의한 수정을 방지

## CFG - Control Flow Guard

- 간접 호출 대상의 유효성을 런타임에 검증해 ROP(Return-Oriented Programming) 공격을 방지

## ASLR - Address Space Layout Randomization

- 실행 파일, DLL, 힙, 스택 주소를 무작위화
- 커널 주소 공간도 ASLR 적용
- 공격자의 특정 주소 예측을 어렵게 함

## DAM - Desktop Activity Moderator

- Win32 앱과 서비스의 스로틀링, 타이머 가상화, 타이머 동결 등 유휴 동작을 관리
- 잡을 통해 구현

## DFSS - Dynamic Fair-Share Scheduling

- 잡을 통해 스케줄링 그룹을 정의하고 CPU 자원을 공정하게 분배하는 동적 스케줄링 메커니즘

## DSCP - Differentiated Services Code Point

- QoS 목적으로 잡이 전송하는 각 네트워크 패킷에 태그를 붙이는 데 사용

## QoS - Quality of Service

- 네트워크 트래픽의 우선순위와 대역폭을 제어해 서비스 품질을 보장하는 기술

## IOPS - Input/Output Operations Per Second

- 디스크 I/O를 대역폭(바이트) 또는 초당 I/O 작업 횟수(IOPS) 기준으로 제한 가능

## WIM - Windows Imaging Format

- 컨테이너가 사용하는 별도 OS 복사본 파일 포맷
- Microsoft는 Server Core 이미지와 Nano Server 이미지를 WIM으로 제공

## VReg - Virtual Registry

- 컨테이너가 임시 레지스트리 하이브 세트와 네임스페이스 격리를 제공하는 커널 컴포넌트
- 컨테이너 종료 시 변경 사항 제거

## LUID - Locally Unique Identifier

- 시스템 재부팅 내에서만 고유한 64비트 식별자
- 로그온 세션, 권한(Privilege) 등을 식별하는 데 사용

## SLS - Silo-Local Storage

- 서버 사일로마다 독립적으로 유지되는 저장 배열
- TLS(스레드 로컬 스토리지)와 유사한 개념
- 같은 슬롯 인덱스에서 사일로마다 다른 포인터를 저장

---

# 4장 줄임말

## ETHREAD - Executive Thread Structure

- "executive thread object encapsulates an ETHREAD structure"
- KTHREAD를 첫 번째 멤버로 포함
- 시스템 주소 공간에 존재 (TEB 제외)

## TIB - Thread Information Block

- TEB의 헤더 부분
- OS/2 및 Win9x 애플리케이션과의 호환성을 위해 존재
- 예외·스택 정보를 담는 작은 구조체

## WSRM - Windows System Resource Manager

- Windows Server 2012 R2 이상에 포함된 옵션 컴포넌트
- 프로세스별 CPU 사용률·친화도·메모리 한도 정책을 설정
- 리소스 사용 보고서 생성 가능

## SMT - Symmetric Multithreading

- 단일 물리 코어가 두 개 이상의 논리 프로세서를 제공하는 기술
- Intel 하이퍼-스레딩, AMD SMT 등
- 쿼텀 만료 시 스케줄러가 SMT 세트 상태를 고려해 프로세서를 선택

---

# 5장 줄임말

## TLB - Translation Lookaside Buffer

- CPU 내부의 가상→물리 주소 변환 캐시
- 용량이 매우 작아 대형 페이지를 사용하면 동일한 가상 범위를 더 적은 TLB 엔트리로 커버
- TLB 미스 → 페이지 테이블 워크(page table walk) 발생

## PFN - Page Frame Number

- 물리 메모리의 각 페이지를 식별하는 번호
- PFN 데이터베이스 : 시스템의 모든 물리 페이지 상태(제로/빈/대기/변경/불량 등)를 추적

## ACL - Access Control List

- "Shared memory section objects have standard Windows access control lists (ACLs)"
- 파일·프린터 등 객체에 부착된 접근 허용/거부 목록

## DEP - Data Execution Prevention

- "Data Execution Prevention (DEP), or no-execute (NX) page protection"
- 데이터 영역에서 코드가 실행되는 것을 방지
- PE 헤더의 IMAGE_DLLCHARACTERISTICS_NX_COMPAT 플래그로 활성화

## MMU - Memory Management Unit

- "ignored by the memory management unit (MMU) inside the CPU"
- CPU 내부에서 가상→물리 주소 변환을 수행하는 하드웨어
- PTE(페이지 테이블 엔트리)를 해석하여 변환

## SID - Security Identifier

- "each one representing a given user SID"
- 사용자, 그룹, 컴퓨터 계정을 고유하게 식별하는 가변 길이 구조체
- 접근 제어(ACL)에서 주체를 식별하는 데 사용

## MFT - Master File Table

- "faults taken on the NTFS master file table (MFT) metadata file"
- NTFS 파일 시스템의 핵심 메타데이터 구조
- 볼륨의 모든 파일·디렉터리 정보를 레코드로 저장

## SGX - Software Guard Extensions

- "Intel Software Guard Extensions (SGX)"
- 프로세서 하드웨어가 직접 보호하는 메모리 엔클레이브 기술
- OS·하이퍼바이저도 내부를 읽을 수 없는 강력한 격리 제공

## AES - Advanced Encryption Standard

- "written using Advanced Encryption Standard (AES) encryption"
- SuperFetch가 디스크에 기록하는 추적 데이터를 부팅 세션 키로 AES 암호화
- 대칭 키 암호화 표준

## COW - Copy-on-Write

- 공유 페이지를 수정하려 할 때 OS가 해당 프로세스만의 개인 복사본을 만드는 메커니즘
- DLL 코드는 여러 프로세스가 공유하지만, 쓰기 가능 데이터 페이지는 COW로 분리

## EPT - Extended Page Tables

- 게스트 OS 페이지 테이블과 별도로 동작하는 두 번째 주소 변환 계층
- 하이퍼바이저가 게스트 물리 주소(GPA)를 실제 물리 주소(SPA)로 변환

## IPC - Inter-Process Communication

- 프로세스 간 데이터를 공유하기 위한 다양한 메커니즘의 총칭
- 공유 메모리 섹션, 파이프, ALPC, RPC 등이 모두 IPC의 구현 형태

---
 
# 6장 줄임말
 
## FAT - File Allocation Table
 
- Windows가 지원하는 설치형 파일 시스템 중 하나
- FAT12, FAT16, FAT32, exFAT 변형 포함
- USB 드라이브, 플래시 카드 등에 주로 사용
- NTFS 대비 보안·복구·대용량 기능 없음
 
## CDFS - CD-ROM File System
 
- CD-ROM 미디어 전용 파일 시스템
- ISO 9660 표준 기반
- 읽기 전용 파일 시스템
 
## UDF - Universal Disk Format
 
- CD, DVD, Blu-ray 등 광학 미디어를 위한 표준 파일 시스템
- CDFS보다 더 많은 기능 지원 (긴 파일명, 유니코드 등)
- DVD-RAM 같은 쓰기 가능 광학 미디어 지원
 
## ReFS - Resilient File System
 
- Windows Server 2012에서 도입된 최신 파일 시스템
- 데이터 무결성 자동 검증 (저장 시 체크섬 계산)
- 대용량 볼륨·파일 지원, 가상화 환경 최적화
- 전체 NTFS 기능(예: 압축)은 지원하지 않음
 
## INF - Device Installation File
 
- 확장자 .inf. 하드웨어와 드라이버의 연결 정보를 담은 스크립트 형식 파일
- 장치 ID, 드라이버 파일 위치, 레지스트리 수정 사항, 의존성 정보 포함
- .cat 파일에 디지털 서명 저장 → 드라이버 변조 방지
 
## IRP - I/O Request Packet

- I/O 요청을 표현하는 핵심 데이터 구조
- I/O 시스템이 패킷 기반으로 동작하는 핵심 단위
- 주 함수 코드 (IRP_MJ_READ 등), 상태, MDL 포인터, I/O 스택 위치 배열을 포함
- Fast I/O만 IRP를 사용하지 않는 예외
- 프로세서별 룩어사이드 리스트에서 할당 (소형 1개, 중형 4개, 대형 14개 스택 위치)
 
## DIRQL - Device Interrupt Request Level
 
- 하드웨어 인터럽트에 할당되는 IRQL 수준
- x86에서 3~26, x64·ARM에서 3~12
- 항상 DISPATCH_LEVEL(2)보다 높으므로 IRQL 2의 제약이 모두 적용됨 (페이지 폴트 불가, 스케줄러 작동 불가)
- ISR이 실행되는 IRQL 수준
 
## ISR - Interrupt Service Routine

- 하드웨어 인터럽트 발생 시 커널 트랩 디스패처가 제어를 넘기는 드라이버 루틴
- DIRQL에서 실행 → 최소한의 작업만 수행 (장치 상태 읽기, 인터럽트 신호 해제)
- 나머지 처리는 DPC로 위임
- 인터럽트 기반 장치에만 존재 (파일 시스템 드라이버에는 없음)
- 특정 스레드 컨텍스트에 의존하는 코드 작성 불가 (임의 스레드에서 실행됨)
 
## USB - Universal Serial Bus

- 범용 직렬 버스. 표준화된 플러그 앤 플레이 주변기기 연결 인터페이스
- UMDF의 적합한 대상 프로토콜 중 하나
 
## PCMCIA - Personal Computer Memory Card International Association

- 노트북 확장 카드 표준 인터페이스 단체 및 표준 명칭
- 현재는 PC Card 또는 ExpressCard로 발전
- PCI, USB, IEEE 1394와 함께 버스 드라이버 예시로 언급
 
## PCI - Peripheral Component Interconnect

- 마더보드와 확장 카드 간의 고속 로컬 버스 표준
- PCIe(PCI Express)로 발전
- 대부분의 데스크톱·서버의 주 확장 버스
 
## IEEE - Institute of Electrical and Electronics Engineers

- 전기전자공학자협회
- IEEE 1394 = FireWire. 고속 직렬 버스 표준. 비디오 카메라, 외장 스토리지 등에 사용
- UMDF의 지원 프로토콜 중 하나
 
## PDO - Physical Device Object

- 버스 드라이버가 PnP 관리자의 지시로 생성하는 장치 오브젝트
- 장치의 물리적 인터페이스 표현
- 디바이스 스택에서 항상 맨 아래 위치 (필수)
- 버스 드라이버가 생성
 
## FDO - Functional Device Object

- 기능 드라이버(Function Driver)가 생성하는 장치 오브젝트
- 장치의 논리적 인터페이스 표현. 장치 기능을 가장 잘 아는 드라이버가 생성
- 디바이스 스택에서 하나만 존재 (필수)
- 애플리케이션이 실제로 통신하는 인터페이스 제공
 
## SATA - Serial Advanced Technology Attachment

- 하드 디스크, SSD를 연결하는 직렬 인터페이스 표준
- 포트 드라이버(Ataport.sys)가 관리
- PATA(병렬 ATA)를 대체
 
## NDIS - Network Driver Interface Specification

- 네트워크 어댑터 드라이버와 프로토콜 스택 사이의 표준 인터페이스
- 미니포트 드라이버(네트워크 어댑터 제조사 작성)와 NDIS 사이의 추상화 계층
- TCP/IP 등 프로토콜 드라이버가 NDIS를 통해 하드웨어에 접근
 
## MDL - Memory Descriptor List

- 물리 메모리의 버퍼를 서술하는 데이터 구조
- 드라이버가 DMA 전송, 직접 I/O 버퍼 접근 시 사용
- 가상 주소 대신 물리 페이지 프레임 번호 목록으로 버퍼 표현
 
## DDI - Device Driver Interface

- 드라이버가 커널 I/O 관리자와 상호작용하기 위한 함수 인터페이스 집합
- WDK 문서에 정의됨
- Universal DDI : 여러 Windows 플랫폼(IoT, Mobile, 데스크톱)에서 공통으로 사용 가능한 DDI 집합
 
## HID - Human Interface Device

- 마우스, 키보드, 게임 컨트롤러 등 사람이 직접 조작하는 입력 장치 클래스
- USB HID 클래스로 표준화 → 제조사별 드라이버 없이도 기본 동작
- UMDF의 지원 프로토콜 중 하나
 
## ACPI - Advanced Configuration and Power Interface

- 하드웨어 리소스 구성, 전원 관리, 열 관리, 플러그 앤 플레이를 위한 개방형 표준
- UEFI의 일부. 시스템 전원 상태(S0~S5)와 장치 전원 상태(D0~D3) 정의
- BIOS의 고정된 전원·구성 관리를 OS가 동적으로 제어할 수 있도록 대체
 
## MUP - Multiple UNC Provider

- UNC 기반 경로(\\서버명\공유명)를 원격 리소스로 연결하는 드라이버
- Start 값 auto-start(2)를 사용하는 비PnP 파일 시스템 드라이버의 대표 예시
 
## SCSI - Small Computer System Interface

- 스토리지 장치(HDD, 테이프, 광학 드라이브, 스캐너 등)를 연결하는 병렬 인터페이스 표준
- 현재는 SAS(Serial Attached SCSI)로 발전
- 미니포트 드라이버 개념의 전통적 구현체
 
## ISA - Industry Standard Architecture

- 구형 PC 확장 버스 표준. PnP를 지원하지 않는 대표적 레거시 인터페이스
- ISA 사운드 카드 등은 자동 감지 불가 → 일부 PnP 기능 사용 불가
 
## UNC - Universal Naming Convention

- 네트워크 공유 자원을 \\서버명\공유명 형식으로 표현하는 경로 규칙
- MUP 드라이버가 UNC 경로를 실제 네트워크 프로바이더(SMB 등)로 라우팅
 
## PoFx - Power Management Framework

- Windows 8에서 도입. 장치 내부의 개별 컴포넌트 단위 전원 상태 관리
- F 상태(F0=완전 동작~Fn=저전력) 관리 — D0 상태에서만 의미 있음
- Windows 10에서 성능 상태 관리(주파수·대역폭·커스텀) 기능 추가
- `PoFxRegisterDevice` → `PoFxActivateComponent` / `PoFxIdleComponent`로 제어
 
## PEP - Platform Extension Plug-in

- 특정 프로세서 계열 또는 SoC에 특화된 전원 관리 플러그인
- Windows 8.x에서 드라이버가 성능 상태 변경을 PEP에 직접 통지해야 했음
- Windows 10 PoFx 확장으로 드라이버가 PEP에 직접 의존하지 않도록 추상화
 
## SoC - System on a Chip

- CPU, GPU, 메모리 컨트롤러, I/O 인터페이스 등을 단일 칩에 통합한 설계
- 모바일 기기, IoT 장치에 주로 사용
- Windows 10 ARM 기반 디바이스(Surface Pro X 등)가 SoC 기반

---
 
# 7장 줄임말
 
## CC - Common Criteria

- 미국·유럽 등 다수 국가가 채택한 현행 국제 보안 평가 표준
- EAL(Evaluation Assurance Level) 1~7 등급으로 평가
- Windows는 CAPP 프로파일 하에 EAL 4+ 인증 획득

## TCSEC - Trusted Computer System Evaluation Criteria

- 미국 국방부 오렌지북(Orange Book)으로 불림
- D(최저) ~ A1(최고) 등급 체계. Windows NT 설계 목표는 C2 등급 달성
- CC로 대체됐지만 Windows 보안 설계의 역사적 기반

## CAPP - Controlled Access Protection Profile
 
- CC에서 정의한 보안 기능 프로파일 중 하나
- C2 등급과 대략 동등한 접근 제어 보호 요구사항을 명시

## EAL - Evaluation Assurance Level

- CC의 보증 등급 체계 (EAL 1~7)
- EAL 4 : 국가 경계를 초월해 인정되는 가장 높은 등급
- EAL 4+ : EAL 4에 결함 수정 보증이 추가된 등급

## Lsass - Local Security Authority Subsystem Service

- Lsass.exe로 실행되는 유저 모드 프로세스
- 로컬 보안 정책, 사용자 인증, 보안 감사 메시지 관리
- Lsasrv.dll이 대부분의 기능 구현. SAM·Active Directory·Kerberos 패키지를 로드

## KPP - Kernel Patch Protection
 
- PatchGuard라고도 불림
- x64·ARM Windows에서 커널 코드·구조체 무단 수정을 감지해 BSOD로 대응
- 공격을 막는 것이 아닌 감지·충돌로 대응하는 메커니즘 (CCTV 비유)

## HVCI - HyperVisor Code Integrity

- Device Guard의 핵심 기술
- SLAT(하이퍼바이저 2차 주소 변환)를 통해 코드 서명 검증을 VTL 1에서 수행
- 커널이 침해되더라도 서명되지 않은 코드 로드·실행 불가

## KMCI - Kernel-Mode Code Integrity

- 커널 모드 드라이버 서명을 강제하는 코드 무결성 서비스
- HVCI가 KMCI를 VTL 1에서 강화

## NTOWF - NT One-Way Function

- 비밀번호의 MD4 해시. NTLM 프로토콜에서 사용자 인증에 활용
- 현대 시스템에서는 Kerberos로 대체됐으나 레거시 컴포넌트가 여전히 사용
- 인터셉트 시 즉시 사용 가능. MD4 취약성으로 비밀번호 복원도 가능

## MD4 - Message Digest 4

- Ron Rivest가 설계한 암호화 해시 알고리즘
- Windows NTLM 인증에서 NTOWF(비밀번호 해시) 생성에 사용
- 현재는 암호학적으로 취약해 보안 목적으로 부적합

## RDP - Remote Desktop Protocol

- Microsoft의 원격 데스크톱 연결 프로토콜
- WDigest와 함께 평문 인증을 사용하는 프로토콜 예시로 언급
- Credential Guard가 이러한 프로토콜에 대한 SSO 기능을 비활성화

## UMCI - User-Mode Code Integrity

- 사용자 모드 이미지(.exe, .dll)의 서명을 강제하는 코드 무결성 서비스
- HVCI가 SLAT를 통해 서명되지 않은 사용자 모드 페이지 실행도 차단

## CCI - Custom Code Integrity

- 기업 관리자가 직접 정의하는 코드 서명 정책
- Secure Boot로 보호되며 인증서 서명자나 SHA-2 해시 기반 포함·제외 목록 정의

## SHA - Secure Hash Algorithm
 
- 미국 NIST가 표준화한 암호화 해시 알고리즘 계열
- SHA-1(160비트, 현재 취약), SHA-2(SHA-256/384/512), SHA-3 포함
- AppLocker·Device Guard·코드 서명 등 Windows 보안 전반에서 파일 무결성 확인에 활용

## MBEC - Mode-Based Execution Control

- SLAT 페이지 테이블 엔트리에 유저 모드·커널 모드 별도 실행 비트를 추가하는 하드웨어 기능
- MBEC가 없는 하드웨어에서는 Restricted User Mode(RUM)로 소프트웨어 에뮬레이션

## RUM - Restricted User Mode

- MBEC를 지원하지 않는 하이퍼바이저가 MBEC 기능을 소프트웨어로 에뮬레이션하는 방식
- Device Guard의 유저/커널 실행 모드 분리를 MBEC 없이 구현

## SKCI - Secure Kernel Code Integrity

- VTL 1(보안 커널)에 위치한 코드 무결성 라이브러리
- Device Guard의 모든 코드 서명 강제를 VTL 1에서 최종 수행

## DACL - Discretionary Access Control List

- 보안 서술자의 핵심 구성 요소. 오브젝트 접근 허용·거부 ACE 목록
- NULL DACL = 모두 허용 (AppContainer에서는 예외적으로 거부 처리)
- 빈(Empty) DACL = 모두 거부

## UIPI - User Interface Privilege Isolation

- 낮은 무결성 프로세스가 높은 무결성 프로세스의 윈도우에 메시지를 보내거나 훅을 설치하는 것을 차단
- UAC와 함께 도입된 UI 격리 메커니즘. AppContainer에도 적용

## RID - Relative Identifier

- SID의 마지막 부분. 도메인·컴퓨터 내에서 계정을 고유하게 식별하는 값
- Administrator = 500, Guest = 501, 일반 계정 = 1000부터 증가
- 같은 도메인 SID + 다른 RID = 다른 계정

## ACE - Access Control Entry

- DACL 또는 SACL을 구성하는 개별 항목
- 구성 : 유형(허용/거부/감사) + SID + 접근 마스크
- 거부 ACE가 허용 ACE보다 항상 우선 적용

## MMC - Microsoft Management Console 

- Windows 관리 도구를 통합하는 호스트 애플리케이션 프레임워크
- 스냅인(snap-in) 형태로 다양한 관리 도구 추가 가능 (보안 정책, 장치 관리자 등)
- secpol.msc(로컬 보안 정책), services.msc, compmgmt.msc 등이 MMC 스냅인

## SACL - System Access Control List

- 보안 서술자의 구성 요소. 어떤 사용자의 어떤 작업을 보안 감사 로그에 기록할지 정의
- SACL을 설정·조회하려면 SeSecurityPrivilege 필요
- 전역 감사 정책(Global Audit Policy)으로 모든 파일/레지스트리에 일괄 적용 가능

## SDDL - Security Descriptor Definition Language

- 보안 서술자를 컴팩트한 문자열로 표현하는 언어
- AppLocker 규칙, 오브젝트 보안 설정 등에 사용
- 예 : `D:(XD;;FX;;;SID;(APPID://FQBN >= {..."}))`

## UAP - Universal Application Platform

- UWP(Universal Windows Platform)의 이전 명칭
- Windows 8에서 처음 도입된 앱 플랫폼으로 다양한 Windows 10 폼팩터에서 실행

## OID - Object Identifier

- 인증서에서 특정 오브젝트 유형(예: EKU)을 고유하게 식별하는 숫자 문자열
- AppContainer capability SID와 인증서 OID를 매핑하는 레지스트리 키에서 사용

## CRT - C Runtime (Universal CRT)

- C언어 런타임 라이브러리. Windows 10부터 uCRT(Universal CRT)로 통합
- Device Guard 컨텍스트에서 NGEN.EXE와 함께 동적 코드 생성이 허용되는 예시로 언급

## DCOM - Distributed Component Object Model

- COM을 네트워크상의 원격 오브젝트 접근을 지원하도록 확장한 기술
- AppLocker의 AppID(Application Identity)와 DCOM의 AppID는 다른 개념임을 명시하는 맥락

## FQBN - Fully Qualified Binary Name

- 서명된 파일의 전체 이진 이름 : `{게시자\제품명\파일명, 버전}` 형식
- AppLocker의 서명 인증서 기반 규칙에서 파일을 식별하는 핵심 속성

## SRP - Software Restriction Policies

- Windows XP에서 도입된 초기 코드 실행 제어 메커니즘
- AppLocker로 대체됐지만 공존 (같은 GPO에서는 AppLocker 규칙 우선)
- 단점 : 사용자·그룹별 적용 불가, 감사 모드 없음

## GPO - Group Policy Object

- Active Directory 도메인에서 컴퓨터·사용자 설정을 관리하는 정책 컨테이너
- AppLocker 규칙을 도메인 전체에 배포할 때 활용
- secpol.msc(로컬)나 gpmc.msc(도메인)로 편집

## OCX - OLE Control Extension

- ActiveX 컨트롤의 파일 확장자. COM 기반 재사용 가능 UI 컴포넌트
- AppLocker가 DLL과 함께 제어 대상으로 지정

## MSI - Microsoft Software Installer (Windows Installer Package)

- Windows 설치 패키지 파일 형식. 설치·제거·업데이트 기능 포함
- AppLocker가 설치 및 제거 동작 모두 제어 가능

## PS1 - PowerShell Script

- PowerShell 스크립트 파일 확장자
- AppLocker가 BAT, CMD, VBS, JS와 함께 스크립트 실행 제어 대상으로 포함

## BAT - Batch Script

- Windows 배치 스크립트 파일 확장자
- AppLocker의 스크립트 제어 대상

## CMD - Command Script

- Windows 명령 스크립트 파일 확장자 (.cmd)
- BAT와 동일한 배치 스크립트이나 구분하여 관리

## CCTV - Closed-Circuit Television

- PatchGuard의 역할을 비유하는 데 사용 : 공격을 막는 잠금장치가 아닌 감지·경보 시스템
- KPP는 CCTV처럼 사후 감지 후 알람(BSOD)을 울릴 뿐 공격 자체를 차단하지 않음

## SSDT - System Service Descriptor Table

- 각 시스템 콜 핸들러를 가리키는 함수 포인터 배열 테이블
- 루트킷이 가장 많이 노리는 커널 구조체 중 하나
- PatchGuard가 무단 수정 감지·BSOD로 보호

## LWF - Lightweight Filter

- 네트워크 드라이버 인터페이스(NDIS) 스택에 삽입되는 필터
- 소켓 연산 감시, 원시 이더넷 프레임 데이터 접근 가능
- PatchGuard 대신 활용 가능한 지원 커널 확장 메커니즘 중 하나

## WFP - Windows Filtering Platform

- 네트워크 패킷 필터링을 위한 Windows 표준 플랫폼
- accept, listen, connect, close 등 소켓 연산과 패킷 자체 감시 가능
- 방화벽·IDS 등 보안 소프트웨어가 활용

## NIC - Network Interface Card
 
- 네트워크 인터페이스 카드. 컴퓨터를 네트워크에 연결하는 하드웨어
- LWF를 통해 NIC에서 전선으로 나가는 원시 이더넷 프레임 데이터에 접근 가능

## NPIEP - Non-Privileged Instruction Execution Prevention

- VBS 활성화 시 하이퍼바이저가 구현하는 완화 기법
- SGDT/SIDT/SLDT 명령 실행은 허용하되 실제 커널 주소 대신 프로세서별 고유 가상 값 반환
- KASLR(커널 ASLR) 정보 유출 공격 방어

## LDT - Local Descriptor Table

- CPU의 세그먼트 서술자 테이블 중 하나 (GDT와 별도로 프로세스별 세그먼트 정의)
- SLDT 명령으로 주소를 읽을 수 있어 KASLR 우회에 악용 가능 → NPIEP로 보호

## SGDT / SIDT / SLDT - Store GDT/IDT/LDT Register

- x64 명령어로 각각 GDT, IDT, LDT의 커널 주소를 읽는 명령
- 정보 유출(KASLR 우회)에 악용 가능
- NPIEP 적용 시 실제 주소 대신 프로세서별 고유 가상 값 반환

## KASLR - Kernel Address Space Layout Randomization

- 부팅 시 커널 이미지·드라이버·커널 구조체의 메모리 주소를 무작위화
- 커널 주소를 알아야 실행 가능한 공격을 어렵게 만드는 완화 기법
- NPIEP는 KASLR 우회 가능한 정보 유출 명령으로부터 보호

# 8장 줄임말
 
## TSS - Task State Segment

- CPU별로 하나씩 존재하는 데이터 구조
- 역할 1 : 유저→커널 모드 전환 시 새 커널 스택 포인터(RSP0) 제공
- 역할 2 : IST(Interrupt Stack Table) — NMI, 더블 폴트 등 치명적 예외 전용 스택 제공
- x64에서 GDT에 등록된 세그먼트로, TR(Task Register)이 TSS 위치를 가리킴

## GDTR - Global Descriptor Table Register

- GDT의 기준 주소와 한계(limit)를 저장하는 CPU 내부 레지스터
- LGDT 명령으로 값을 설정, SGDT 명령으로 읽기 (NPIEP가 SGDT를 차단함 — 7장 참조)
- 시스템당 하나가 아닌 CPU당 하나씩 독립된 값을 가짐

## IST - Interrupt Stack Table
 
- TSS 내부에 존재하는 스택 포인터 배열 (IST1~IST7)
- NMI, 더블 폴트, 머신 체크 등 치명적 예외가 발생 시 현재 스택과 무관하게 전용 스택으로 전환
- 스택 오버플로우로 인한 더블 폴트가 다시 스택 오버플로우를 일으키는 문제를 방지

## NMI - Non-Maskable Interrupt

- 하드웨어 오류나 디버거 브레이크 등 소프트웨어로 무시(mask)할 수 없는 인터럽트
- IF 플래그로 차단되지 않음
- IST를 통해 전용 스택에서 처리됨

## KVA - Kernel Virtual Address (Shadow)

- Meltdown 취약점 완화를 위한 Windows 커널 기법
- 유저 모드 실행 중 커널 핵심 코드만 매핑한 섀도우 페이지 테이블로 전환
- 전체 커널 주소 공간은 커널 모드 전환 시에만 노출

## PCID - Process Context ID

- Intel CPU의 TLB 태깅 기능. 프로세스 전환 시 TLB 전체를 무효화하지 않아도 됨
- KVA Shadow 구현에서 유저/커널 모드 페이지 테이블 전환 비용 감소에 활용
- PCID를 지원하지 않는 CPU에서는 매 전환마다 TLB 플러시 발생 → 성능 저하

## IBRS - Indirect Branch Restricted Speculation
 
- Spectre v2 완화를 위한 Intel/AMD CPU 기능
- 커널 모드에서 유저 모드의 분기 예측 테이블이 커널 코드 실행에 영향을 주지 못하게 차단
- 성능 저하가 크기 때문에 Retpoline으로 대체되거나 Enhanced IBRS(eIBRS)로 개선됨

## IBPB - Indirect Branch Predictor Barrier

- 컨텍스트 전환 시 분기 예측 테이블을 초기화하는 CPU 펜스(barrier) 명령
- 이전 프로세스의 분기 예측 상태가 새 프로세스의 투기 실행에 영향을 주지 못하게 차단

## STIBP - Single Thread Indirect Branch Predictors
 
- SMT(하이퍼스레딩) 환경에서 같은 물리 코어의 형제 논리 프로세서 간 분기 예측 격리
- 악의적인 유저 스레드가 같은 코어의 다른 스레드(커널 포함)를 분기 예측 오염으로 공격하는 것 방지

## SSBD - Speculative Store Bypass Disable
 
- Spectre v4(Speculative Store Bypass) 완화를 위한 CPU 기능
- 이전 스토어 명령의 값을 우회(bypass)하는 투기적 로드를 비활성화
- 성능 저하가 있어 필요한 경우에만 선택적으로 활성화

## IDTR - Interrupt Descriptor Table Register
 
- IDT의 기준 주소와 한계(limit)를 저장하는 CPU 내부 레지스터
- LIDT 명령으로 설정, SIDT 명령으로 읽기 (NPIEP가 SIDT를 차단함 — 7장 참조)
- CPU당 독립된 IDTR을 가짐

## HPET - High Precision Event Timer
 
- 고해상도 하드웨어 타이머. 100나노초 단위 정밀도
- 구형 PIT(Programmable Interval Timer)를 대체
- `QueryPerformanceCounter` API의 내부 소스 중 하나

## TSC - Time Stamp Counter
 
- CPU 클록 사이클을 카운트하는 64비트 레지스터. RDTSC 명령으로 읽기
- 가장 빠른 타임스탬프 소스 (단일 명령으로 읽기 가능)
- Invariant TSC : 전원 절약 모드에서도 일정 주파수로 증가하는 개선된 버전. `QueryPerformanceCounter`가 이를 활용

## SEH - Structured Exception Handling
 
- Windows의 구조적 예외 처리 메커니즘. C언어의 __try/__except/__finally 키워드로 사용
- 예외 발생 시 SEH 프레임 체인을 순서대로 탐색해 처리기를 찾음
- SEHOP(SEH Overwrite Protection)으로 SEH 체인 변조 탐지

## VEH - Vectored Exception Handling
 
- SEH보다 먼저 호출되는 전역 예외 핸들러 목록 기반 메커니즘
- `AddVectoredExceptionHandler`로 핸들러 등록
- SEH가 처리하지 못한 예외가 VEH에 전달되는 것이 아니라, SEH보다 먼저 시도됨

## KMUTEX - Kernel Mutex
 
- 커널 모드의 뮤텍스 오브젝트를 나타내는 내부 구조체 이름
- 소유자 개념 있는 상호 배제. 소유 스레드가 재진입 가능
- 소유 스레드가 종료되면 abandoned 상태가 되어 대기 중인 스레드가 WAIT_ABANDONED로 깨어남

## KEVENT - Kernel Event

- 커널 모드 이벤트 오브젝트의 내부 구조체 이름
- 알림 이벤트(Notification) : 수동 리셋, 신호 시 모든 대기 스레드 해제
- 동기화 이벤트(Synchronization) : 자동 리셋, 신호 시 대기 스레드 하나만 해제

## KPCR - Kernel Processor Control Region

- CPU당 하나씩 존재하는 커널 프로세서 제어 구조체
- GS 세그먼트 레지스터가 현재 CPU의 KPCR 주소를 가리킴
- KPRCB(Kernel Processor Control Block)를 포함. IRQL, 현재 스레드, DPC 큐, 통계 정보 등 저장

## WNF - Windows Notification Facility

- Windows의 커널 통합 발행-구독 알림 메커니즘 (Windows 8~)
- 커널과 유저 모드 모두 상태 변경을 발행·구독 가능
- 저장소 유형 : Temporary, Permanent, Runtime, Device State
- 동일 시간대 다수 변경을 집계(aggregate)해 콜백 호출 횟수 최소화

## PLM - Process Lifetime Manager
 
- UWP 앱의 실행 생명주기를 제어하는 시스템 컴포넌트
- 앱 상태 : 실행(Running) → 일시 중단(Suspended) → 종료(Terminated) 전환 관리
- 메모리 압박 시 백그라운드 앱을 자동으로 종료
- HAM과 협력해 앱 활성화·비활성화 조율

## HAM - Host Activity Manager

- UWP/Centennial 패키지 앱의 활성화·비활성화 생명주기를 관리하는 컴포넌트
- PLM의 요청을 받아 실제 앱 프로세스 생성·활성화를 수행
- 백그라운드 작업(Background Broker Infrastructure)과 연동해 트리거 기반 실행 조율

# 9장 줄임말
 
## VP - Virtual Processor
 
- 하이퍼바이저가 파티션에 제공하는 가상 프로세서
- 루트 파티션의 Windows 스레드(백킹 스레드)로 구현됨
- VM_VP 데이터 구조가 레지스터 상태, 전용 주소 공간, VTL별 상태를 추적
- VP는 가상 APIC와 타이머를 독립적으로 보유
## GPA - Guest Physical Address
 
- 게스트 OS(파티션)가 "물리 주소"라고 인식하는 가상 주소
- SLAT(EPT/NPT/Stage-2)를 통해 실제 물리 주소(SPA)로 변환
- 루트 파티션은 Identity Mapping(GPA = SPA), 차일드는 GPA ≠ SPA
## SPA - System Physical Address
 
- 실제 물리 메모리의 주소
- SLAT 변환의 최종 목적지
- 하이퍼바이저가 GPA → SPA 매핑 테이블(EPT/NPT)을 관리
## EPT - Extended Page Tables
 
- Intel CPU의 SLAT 구현 명칭
- 게스트 물리 주소(GPA)를 시스템 물리 주소(SPA)로 변환하는 4단계 페이지 테이블
- VTL 0과 VTL 1에 각각 독립된 EPT 테이블이 존재
- MBEC 지원 시 유저/커널 모드 실행 비트가 EPT 항목에 추가됨
## NPT - Nested Page Tables
 
- AMD CPU의 SLAT 구현 명칭
- Intel EPT와 동일한 목적을 수행하는 AMD 전용 2단계 주소 변환
## VMMS - Virtual Machine Management Service
 
- vmms.exe. 가상 머신 관리 서비스
- VM 생성·시작·중지·마이그레이션 등 생명주기 전반을 조율
- WMI Provider를 통해 Hyper-V 관리 API를 외부에 노출
## VMWP - Virtual Machine Worker Process
 
- vmwp.exe. VM당 하나씩 생성되는 가상 머신 워커 프로세스
- VM 상태 관리, 장치 에뮬레이션, 하이퍼바이저와의 통신 담당
- VM Worker Process 내부에 Vmdynmem.dll 등 여러 모듈 포함
## VID - Virtual Infrastructure Driver
 
- VID.sys. 하이퍼바이저와 직접 통신하는 커널 모드 드라이버
- 파티션·VP 생성, 메모리 입출금(HvDepositMemory/HvWithdrawMemory), 인터셉트 처리
- 파티션 생성 시 권한(Privilege) 설정도 VID가 담당
## VSP - Virtualization Service Provider
 
- 루트 파티션 측에서 가상 디바이스를 제공하는 컴포넌트
- 네트워크 VSP(NetVSP), 스토리지 VSP(StorVSP) 등
- VMBus를 통해 차일드 파티션의 VSC와 통신
## VSC - Virtualization Service Client
 
- 차일드 파티션 측의 가상 디바이스 클라이언트 드라이버
- 실제 하드웨어 드라이버처럼 동작하지만, 내부적으로 VMBus로 루트의 VSP에 I/O 전달
## VMBus - Virtual Machine Bus
 
- 루트 파티션의 VSP와 차일드 파티션의 VSC를 연결하는 고성능 가상 버스
- 대부분의 I/O(네트워크, 스토리지, 그래픽 등)가 VMBus를 통해 처리
- 하이퍼콜 기반 공유 메모리 채널 방식으로 낮은 오버헤드 제공
## BSP - Bootstrap Processor
 
- 시스템 부팅 시 가장 먼저 실행되는 프로세서
- 하이퍼바이저 초기화 후 루트 파티션의 첫 번째 VP(BSP VP)를 생성해 부팅 과정을 재개
- AP(Application Processor)와 구분됨
## AP - Application Processor
 
- BSP를 제외한 나머지 물리 CPU
- BSP VP가 생성·초기화된 이후 순차적으로 시작됨
- 하이퍼바이저가 각 AP에 대해 별도의 CPU_PLS와 VP를 초기화
## CPU_PLS - Physical Processor Local Storage
 
- 하이퍼바이저에서 물리 프로세서를 나타내는 데이터 구조
- NT 커널의 KPRCB에 대응하는 하이퍼바이저 내부 구조체
- GS 세그먼트로 빠르게 접근 (NT 커널이 KPCR을 GS로 접근하는 것과 동일한 방식)
## VAL - Virtualization Abstraction Layer
 
- 하이퍼바이저가 Intel/AMD/ARM64의 하드웨어 가상화 확장 차이를 추상화하는 계층
- Intel 전용 코드(EPT, SGX, MBEC 관리 등)와 AMD, ARM64 전용 코드를 분리해 관리
## SynIC - Synthetic Interrupt Controller
 
- 하이퍼바이저가 제공하는 가상 인터럽트 컨트롤러
- 물리 APIC를 에뮬레이션하는 대신 하이퍼콜 기반으로 효율적으로 인터럽트 전달
- 파티션 간 메시지 전달과 이벤트 신호에도 활용됨
## VPID - Virtual Processor Identifier
 
- TLB 엔트리에 VP 식별자를 태깅해 VM 전환 시 전체 TLB 플러시를 피하는 하드웨어 기능
- Intel VT-x의 기능. AMD에서는 ASID(Address Space Identifier)로 대응
- 중첩 가상화 환경에서 L1↔L0 전환 성능 저하를 완화
## HvLoader - Hypervisor Loader
 
- hvloader.dll. 하이퍼바이저 바이너리를 로드하고 하이퍼바이저 로더 블록을 생성하는 모듈
- CPU 제조사에 맞는 하이퍼바이저 이미지를 선택해 로드
- 하이퍼바이저가 시작하기 전에 물리 페이지를 사전 할당하고 예약
## VMENTER / VMEXIT
 
- VMENTER : CPU가 게스트 VM의 컨텍스트로 전환하는 하드웨어 이벤트
- VMEXIT : 하이퍼바이저 개입이 필요한 이벤트(인터럽트, 하이퍼콜, 특정 명령 실행 등)로 인해 게스트에서 하이퍼바이저로 제어가 넘어가는 이벤트
- VMEXIT 처리 비용이 가상화 오버헤드의 핵심
## VMXROOT
 
- Intel VT-x 용어. 하이퍼바이저 자체가 실행되는 최상위 특권 모드
- VMXROOT에서는 일반적인 ring 0/3 개념이 적용되지 않음
- 하이퍼바이저가 실행 중일 때의 CPU 실행 컨텍스트
## Hypercall
 
- 게스트 파티션(또는 루트 파티션)이 하이퍼바이저 서비스를 요청하는 인터페이스
- x64에서 VMCALL 명령으로 발행
- 하이퍼콜 번호(function code) + 입력 파라미터(레지스터 또는 메모리)로 구성
## GPC - Guest Physical to Child Partition
 
GPA와 함께 사용되는 용어. 차일드 파티션에서 게스트 물리 주소 공간이 하이퍼바이저에 의해 격리됨을 나타냄
 
## HyperClear
 
- Intel/AMD 사이드채널 취약점(Spectre, Meltdown, Foreshadow)에 대응하는 Hyper-V의 VM 간 격리 완화 기법
- 세 가지 구성 요소 : 코어 스케줄러 + VP 전용 주소 공간 격리 + 민감 데이터 스크러빙
- 코어 스케줄러가 같은 VM의 VP만 동일 SMT 코어에서 실행하도록 보장
## SR-IOV - Single Root I/O Virtualization
 
- PCIe 장치의 하드웨어 가상화 기능
- 하나의 물리 장치에서 여러 개의 가상 기능(VF)을 생성
- VF를 차일드 파티션에 직접 할당하면 VMBus 경로를 우회해 베어메탈에 가까운 I/O 성능 달성
## L0 / L1 하이퍼바이저
 
- L0 : 실제 물리 하드웨어를 관리하는 루트(외부) 하이퍼바이저
- L1 : L0의 VM 안에서 실행되는 게스트(내부) 하이퍼바이저
- 중첩 가상화(Nested Virtualization) 환경에서의 용어
## VDEV - Virtual Device
 
- 가상화 스택이 차일드 파티션에 노출하는 가상 하드웨어 장치
- 네트워크 어댑터, 디스크 컨트롤러, 그래픽 카드 등을 소프트웨어로 에뮬레이션
## VMMEM
 
- vmmem.exe. VM의 메모리 사용량을 작업 관리자에 표시하기 위한 프로세스
- 실제로 VM에 할당된 물리 메모리를 추적하는 시스템 프로세스
## KPTI - Kernel Page Table Isolation
 
- KVA Shadow와 동일한 개념의 Linux 커뮤니티 용어
- Meltdown 완화를 위해 유저 모드 실행 시 커널 페이지 테이블을 최소화하는 기법
- Windows에서는 KVA Shadow라고 부름
## MDS - Microarchitectural Data Sampling
 
- CPU 내부 버퍼(라인 채우기 버퍼, 로드 포트, 저장소 포워딩 버퍼)에서 데이터를 추론 읽기하는 사이드채널 취약점 계열
- RIDL, Fallout, ZombieLoad 등이 포함됨
- 완화 : MD_CLEAR 기능(VERW 명령으로 버퍼 플러시)
## L1TF - L1 Terminal Fault (Foreshadow)
 
- L1 캐시에 남아있는 데이터를 투기 실행으로 읽는 사이드채널 공격
- SGX 엔클레이브 내부 데이터, 하이퍼바이저 메모리, 커널 메모리를 게스트에서 읽을 수 있는 취약점
- 완화 : L1D 캐시 플러시(VM 전환 시), 코어 스케줄러 적용
## SKINIT - Secure Kernel INITialization
 
- AMD CPU의 Secure Launch 기능
- TPM과 협력해 신뢰할 수 있는 실행 환경을 처음부터 부트스트랩
- Intel TXT(Trusted Execution Technology)의 AMD 대응 기술
## TXT - Trusted Execution Technology
 
- Intel의 하드웨어 기반 신뢰 실행 기술
- DRTM(Dynamic Root of Trust Measurement)을 제공
- Secure Launch(측정 부팅의 고급 형태)에 활용됨

---

# 10장 줄임말

## TxR - Transactional Registry

- KTM + CLFS 기반 레지스트리 트랜잭션 기능
- `RegCreateKeyTransacted` / `RegOpenKeyTransacted`로 커밋·롤백 가능
- NTFS TxF와 같은 KTM 트랜잭션으로 묶어 원자적 파일+레지스트리 변경 가능

## KTM - Kernel Transaction Manager

- 커널 내장 분산 트랜잭션 조율 컴포넌트
- TxR(트랜잭션 레지스트리)과 TxF(트랜잭션 파일 시스템) 모두 KTM 위에 구현
- 2단계 커밋(Two-Phase Commit) 프로토콜 지원

## CLFS - Common Log File System

- Windows Vista~. 고성능 순차 로그 파일 시스템
- TxR, TxF, 클러스터 서비스 등의 로깅 백엔드
- 로그 스트림 단위로 관리. 체크포인트·복구 지원

## SDB - Shim Database

- 앱 호환성 쉼(Shim) 정보를 저장하는 데이터베이스 파일
- `%SystemRoot%\AppPatch\sysmain.sdb`가 기본 시스템 데이터베이스
- 앱 시그니처(이름, 버전, 체크섬)와 적용할 쉼 매핑 정보 포함

## SCP - Service Control Program

- SCM과 통신해 서비스를 제어하는 프로그램
- services.msc, sc.exe, net start/stop 등이 SCP의 예시
- `OpenSCManager`, `OpenService`, `ControlService` API로 구현

## UBPM - Unified Background Process Manager

- Windows 8~. 시스템 전체 백그라운드 작업을 통합 조율하는 인프라
- Task Scheduler, Modern Standby 백그라운드 트리거, UWP 백그라운드 작업을 단일 프레임워크로 통합
- 배터리·네트워크 상태에 따라 백그라운드 작업 실행을 조절

## SEB - System Events Broker

- 시스템 이벤트를 UBPM에 전달하는 이벤트 브로커
- ETW 이벤트, WNF 상태 변경, 네트워크 연결 상태 등 다양한 소스 수집
- UWP 앱 백그라운드 트리거 실행 조율에 핵심 역할

## BI - Background Broker Infrastructure

- UWP 앱의 백그라운드 작업 트리거 등록·실행을 처리하는 인프라
- UBPM과 협력해 배터리·CPU·네트워크 자원을 고려한 백그라운드 작업 스케줄링
- 앱이 PLM에 의해 일시 중단된 상태에서도 특정 트리거에 반응해 짧게 실행 가능

## CIM - Common Information Model

- DMTF가 정의한 객체지향 시스템 관리 정보 모델
- 시스템·네트워크·앱 자원을 계층적 클래스 구조로 표현
- WMI는 Windows에서 CIM을 구현한 서비스

## MOF - Managed Object Format

- WMI 클래스와 인스턴스를 기술하는 언어
- `.mof` 파일로 작성 후 `mofcomp.exe`로 WMI 리포지터리에 등록
- Win32_Process, Win32_Service 등 내장 WMI 클래스가 MOF로 정의됨

## WQL - WMI Query Language

- WMI 클래스 인스턴스를 쿼리하는 SQL 유사 언어
- `SELECT * FROM Win32_Process WHERE Name = 'notepad.exe'` 형식
- WHERE 절 필터링, ASSOCIATORS OF로 관계 탐색 가능

## DMTF - Distributed Management Task Force

- CIM 표준을 관리하는 업계 컨소시엄
- WMI의 CIM 구현 기반 표준을 정의하는 단체

## ETL - Event Trace Log

- ETW 이벤트를 기록한 바이너리 로그 파일 확장자
- `tracerpt.exe`, WPA, PerfView 등으로 분석
- xperf, Windows Performance Recorder(WPR)로 수집

## WPP - Windows Software Trace PreProcessor

- ETW 기반 경량 디버그 추적 프레임워크
- `DoTraceMessage` 매크로를 소스에 삽입하면 컴파일 시 ETW 이벤트 코드 자동 생성
- 비활성화 시 오버헤드 거의 없음

## WPA - Windows Performance Analyzer

- ETL 파일을 시각화하는 GUI 분석 도구
- CPU 사용률, 메모리, I/O, 네트워크, 부팅 타임라인 등을 타임라인 뷰로 표시
- Windows ADK에 포함

## ADK - Windows Assessment and Deployment Kit

- Windows 이미지 커스터마이징·배포·성능 평가 도구 모음
- WPA, WPR, WPRUI, ACT 포함
- 기업 환경 Windows 배포 자동화에 활용

## AeDebug

- 처리되지 않은 예외 발생 시 실행할 디버거를 지정하는 레지스트리 키
- `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug`
- Debugger 값에 디버거 경로, Auto 값으로 자동 실행 여부 결정

## WPR - Windows Performance Recorder

- WPA와 함께 사용하는 ETW 추적 수집 도구
- 프리셋(CPU, 힙, I/O 등)으로 추적 세션 시작·중지
- .wprp 프로파일 XML로 커스텀 추적 구성 가능

## gMSA - Group Managed Service Account

- 도메인 환경에서 여러 서버가 공유하는 관리 서비스 계정
- Active Directory가 비밀번호 자동 갱신
- 단일 서버용 MSA의 그룹 확장 버전

## MSA - Managed Service Account

- 도메인 연동으로 비밀번호를 자동 관리하는 서비스 계정
- 단일 서버에서만 사용 가능
- `New-ADServiceAccount` PowerShell 명령으로 생성

## BCD - Boot Configuration Data

- Windows 부트 로더 옵션 설정 데이터베이스
- `HKLM\BCD00000000` 레지스트리 하이브로 마운트됨
- `bcdedit.exe`로 관리

## DPC Watchdog

- DISPATCH_LEVEL에서 코드가 기준 시간 이상 실행 시 DPC_WATCHDOG_VIOLATION BSOD 발생
- 드라이버가 DISPATCH_LEVEL에서 너무 오래 블로킹하는 버그 탐지

---

# 11장 줄임말

## VACB - Virtual Address Control Block

- 캐시 관리자의 캐시 슬롯 단위 (256KB)
- 파일 오프셋(VBO)과 가상 주소 매핑을 추적하는 데이터 구조
- 시스템 전체 VACB 배열이 캐시 가상 공간 전체를 관리

## VBO - Virtual Byte Offset

- 파일 내의 바이트 위치 (파일 오프셋)
- 캐시 관리자가 VBO 기준으로 VACB를 선택·관리
- 섹션 오브젝트의 매핑 단위

## LBO - Logical Block Address (LBA) / Logical Byte Offset

- 디스크 볼륨 내 실제 디스크 블록 주소
- 파일 시스템이 VBO → LBO 변환을 담당

## FSD - File System Driver

- 파일 시스템 드라이버의 약칭
- Ntfs.sys, Fat.sys 등 로컬 FSD와 Mup.sys·Rdbss.sys 등 원격 FSD로 구분
- I/O 관리자로부터 IRP를 받아 처리

## USN - Update Sequence Number

- NTFS 변경 저널 파일($UsnJrnl)의 레코드 식별 번호
- 파일·디렉터리 생성·수정·삭제·이름 변경 등의 변경 사항을 순서대로 기록
- 백업 소프트웨어, 검색 엔진, 안티멀웨어가 변경 감지에 활용

## LCN - Logical Cluster Number

- NTFS 볼륨 내 클러스터의 논리 번호 (볼륨 시작부터 순서대로)
- 파일의 비거주 속성(Non-Resident)이 LCN으로 디스크 위치를 기록

## VCN - Virtual Cluster Number

- 파일 내 클러스터의 논리 번호 (파일 시작부터 순서대로)
- Run List: VCN → LCN 매핑으로 파일의 디스크 위치 추적

## MFT - Master File Table

- NTFS 볼륨의 모든 파일·디렉터리 레코드를 저장하는 핵심 메타데이터 파일($MFT)
- 볼륨 크기에 따라 사전 예약 (기본 12.5%)
- 첫 16개 레코드는 시스템 메타데이터 파일 전용

## LFS - Log File Service

- NTFS의 로깅 서비스. 메타데이터 변경을 $LogFile에 사전 기록
- 커밋·언커밋 트랜잭션을 관리해 크래시 후 복구 지원
- 쓰기 전 로깅(Write-Ahead Logging, WAL) 구현

## EFS - Encrypting File System

- NTFS 기반 파일 단위 투명 암호화 기능
- FEK(File Encryption Key)를 사용자 인증서 공개 키로 암호화해 $EFS 속성에 저장
- BitLocker(볼륨 암호화)와 독립적으로 사용 가능

## FEK - File Encryption Key

- EFS가 파일 데이터를 암호화하는 데 사용하는 대칭 키 (AES 256비트)
- 파일별로 고유하게 생성
- 사용자 인증서 공개 키로 암호화되어 $EFS 스트림에 저장

## DAX - Direct Access (Disks)

- NVM(Non-Volatile Memory) 장치를 파일 시스템 캐시를 우회하고 직접 접근하는 기능
- NTFS DAX 모드: 캐시 관리자 바이패스, 물리 주소로 직접 파일 매핑
- `FILE_FLAG_DAX_VOLUME`으로 DAX 볼륨에 열린 파일임을 표시

## PM - Persistent Memory (NVM)

- 전원이 꺼져도 데이터를 유지하는 바이트 주소 지정 가능 스토리지
- Intel Optane DC Persistent Memory가 대표 예시
- DAX 모드로 파일 시스템 캐시 없이 직접 접근 가능

## SCM Storage - Storage Class Memory

- PM/NVM 장치를 위한 Windows 스토리지 드라이버 모델
- DAX 드라이버 모델의 기반
- `scmbus.sys`, `scmdisk0901.sys` 등이 SCM 드라이버

## VDL - Valid Data Length

- 파일에서 실제 유효 데이터가 기록된 마지막 바이트 위치
- VDL 이후~EOF 사이는 읽기 시 0으로 채워짐 (초기화 안 된 영역)
- ReFS Sparse VDL: VDL을 빠르게 확장해 초기화 없이 스파스 영역 생성

## SMR - Shingled Magnetic Recording

- 트랙을 겹쳐(shingled) 기록 밀도를 높이는 HDD 기술
- 임의 쓰기 불가, 순차 쓰기만 가능
- ReFS가 SMR 디스크에 맞는 순차 쓰기 최적화 I/O 패턴 제공

## oplock - Opportunistic Lock

- 원격 파일 시스템 클라이언트가 파일을 로컬에 캐시할 수 있도록 허용하는 잠금
- 서버가 충돌 접근 감지 시 클라이언트에 파괴(break) 알림 전송
- 파괴 시 클라이언트는 캐시를 플러시하고 서버에 최신 데이터 기록

## LRC - Local Reconstruction Codes

- Storage Spaces의 패리티 방식 중 하나
- RAID 6보다 효율적인 오류 정정 코드
- 드라이브 2개 장애를 복구할 수 있으면서 패리티 오버헤드를 줄임

---

# 12장 줄임말

## PK - Platform Key

- Secure Boot 계층 구조의 최상위 키
- UEFI 펌웨어 제조사가 소유·관리
- KEK(Key Exchange Key) 업데이트를 허가

## KEK - Key Exchange Key

- Secure Boot에서 허용(db)·거부(dbx) 서명 DB를 업데이트할 수 있는 키
- PK로 서명된 KEK만 DB를 수정 가능
- Microsoft의 KEK가 Windows 부트로더 서명 관리에 사용됨

## PCR - Platform Configuration Register

- TPM 내부의 측정값 저장 레지스터
- 측정값은 누적 해시(extend 연산)로 업데이트됨 → 이전 값 변경 불가
- BitLocker가 PCR 값에 암호화 키를 봉인(seal)

## DRTM - Dynamic Root of Trust Measurement

- 시스템이 실행 중에 새로운 신뢰 루트를 동적으로 확립하는 기술
- Intel TXT 또는 AMD SKINIT로 구현
- Secure Launch가 DRTM을 활용해 부팅 후에도 안전한 hypervisor 시작 가능

## MBR - Master Boot Record

- BIOS 부팅에서 디스크 첫 번째 섹터에 위치하는 부트 코드 및 파티션 테이블
- 최대 4개의 기본 파티션, 2TB 디스크 크기 한도
- UEFI + GPT로 대체됨

## GPT - GUID Partition Table

- UEFI 기반 현대 파티션 테이블 형식
- 128개 파티션, 9.4ZB 디스크 크기 지원
- CRC32 체크섬으로 파티션 테이블 무결성 검증

## VBR - Volume Boot Record

- NTFS/FAT 볼륨의 첫 번째 섹터
- BPB(BIOS Parameter Block)를 포함해 파일 시스템 파라미터 저장
- OS 로더가 볼륨 레이아웃 파악에 사용

## BPB - BIOS Parameter Block

- 볼륨 부트 레코드(VBR)에 포함된 파일 시스템 파라미터 블록
- 섹터 크기, 클러스터당 섹터 수, MFT 위치 등을 정의

## WinRE - Windows Recovery Environment

- Windows가 정상 부팅되지 않을 때 실행되는 복구 환경
- 별도 파티션 또는 WIM 파일로 제공
- 시스템 복원, 시작 복구, 명령 프롬프트, 이미지 복구 등 제공

## ACT - Application Compatibility Toolkit

- 앱 호환성 문제를 진단·수정하는 도구 모음
- Windows ADK에 포함
- Compatibility Administrator: 커스텀 SDB 파일 생성 도구

## iSCSI - Internet Small Computer System Interface

- 네트워크(TCP/IP)를 통해 SCSI 명령을 전송하는 프로토콜
- Winload가 iSCSI 원격 디스크에서 직접 부팅 가능 (Microsoft iSCSI 이니시에이터)
- SAN(Storage Area Network) 환경에서 활용

## SMBIOS - System Management BIOS

- 시스템 하드웨어 정보(프로세서, 메모리, 시리얼 번호 등)를 OS에 전달하는 표준
- Winload가 UEFI 서비스를 통해 수집 후 로더 파라미터 블록에 포함
- `Get-WmiObject Win32_ComputerSystem`으로 일부 SMBIOS 정보 조회 가능

## TxF - Transactional NTFS

- KTM 기반 NTFS 트랜잭션 파일 I/O 기능
- `CreateFileTransacted`, `MoveFileTransacted` 등 트랜잭션 API
- CLFS를 로깅 백엔드로 사용
- Windows Vista~. Windows 8+에서는 레거시로 분류됨

## VSS - Volume Shadow Copy Service

- 파일이 열려 있는 상태에서도 일관성 있는 볼륨 스냅샷을 생성하는 서비스
- 백업, System Restore(시스템 복원), 이전 버전 기능의 기반
- 프로바이더(소프트웨어/하드웨어), 요청자(백업 앱), 기록기(VSS 인식 앱)로 구성

## BCDEdit

- BCD(Boot Configuration Data) 데이터베이스를 관리하는 명령줄 도구
- 부팅 항목 추가·제거, 하이퍼바이저 설정, 디버거 설정, 안전 모드 활성화 등
- 예시: `bcdedit /set hypervisorschedulertype Core`

## Winresume

- 최대절전 또는 Fast Startup 복원 시 실행되는 OS 로더 역할 모듈
- UEFI: `winresume.efi` / BIOS: `winresume.exe`
- Hiberfil.sys에서 커널 상태를 읽어 메모리 복원
