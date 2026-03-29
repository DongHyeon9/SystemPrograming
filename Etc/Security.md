# 1. 보안의 기본 원칙(CIA Triad)
정보보안의 핵심 원칙, 모든 보안 시스템의 기반

## 1-1 기밀성 (Confidentiality)
권한이 있는(인가된) 사용자만 데이터 접근 가능
- 암호화
- 인증(Authentication)
- 권한 관리

예시
- 로그인 시스템
- HTTPS 통신

## 1-2 무결성 (Integrity)
데이터가 변조되지 않음을 보장
- 해시(Hash)
- 디지털 서명

예시
- 패킷 변조 방지
- 파일 무결성 체크

## 1-3 가용성 (Availability)
필요할 때 항상 접근 가능
- 서버 이중화
- 백업
- 로드 밸런싱

예시
- DDOS 대응
- 장애 복구 시스템

# 2. 보안의 추가 개념
### 인증 (Authentication)
"누구냐?"의 확인 -> ID/PW, OTP
### 인가 (Authorization)
"무엇을?"의 확인 -> 관리자 권한
### 부인 방지 (Non-Repudiation)
행동에 대한 부인 방지 -> 전자서명

# 3. 암호화 (Cryptography)
보안에서 가장 중요한 기술

## 3-1. 대칭키 암호화
하나의 키로 암호화/복호화를 수행
### 대표 알고리즘
- AES (https://ko.wikipedia.org/wiki/%EA%B3%A0%EA%B8%89_%EC%95%94%ED%98%B8%ED%99%94_%ED%91%9C%EC%A4%80)
- DES (https://ko.wikipedia.org/wiki/%EB%8D%B0%EC%9D%B4%ED%84%B0_%EC%95%94%ED%98%B8%ED%99%94_%ED%91%9C%EC%A4%80)
- 3DES (https://ko.wikipedia.org/wiki/%ED%8A%B8%EB%A6%AC%ED%94%8C_DES)
### 장점
- 연산이 단순(XOR, 치환 등) -> CPU 부담 적음 -> 대량 데이터 처리에 적합 (파일 암호화)
- 구조가 직관적
### 단점
- 같은 키를 서로 공유해야됨
- 네트워크로 키 전달 -> 탈취가능(MITM, Man-In-The-Middle)

## 3-2. 비대칭키 암호화
Public Key와 Private Key로 암호화
### 대표 알고리즘
- RSA (https://ko.wikipedia.org/wiki/RSA_%EC%95%94%ED%98%B8)
- ECC (https://ko.wikipedia.org/wiki/%ED%83%80%EC%9B%90%EA%B3%A1%EC%84%A0_%EC%95%94%ED%98%B8)
### 장점
- Private Key를 공유할 필요 없음
### 단점
- 매우 느림 -> 복잡한 수학 연산 사용

## 3-3. 해시 함수
- 데이터를 고정 길이로 변환
- 복구 불가능(단방향)
### 대표 알고리즘
- SHA-256 (SHA 계열)
- MD5
### 용도
- 비밀번호 저장(DB)
- 무결성 검증
