# vscode로 아두이노 개발하기
- ``https://juahnpop.tistory.com/71``

#
## **[2021-03-29]**
1. 블로그 보면서 vscode에 세팅

2. uno r3 port가 안잡혀서 다음 과정 진행
    1. 장치 관리자 진입
    2. 기타장치 > USB Serial 우클릭하여 드라이버 업데이트 진행
        - 컴퓨터에서 드라이버 찾기하여 ``C:\Program Files (x86)\Arduino\drivers`` 지정
            > 실패
        - 안돼서 아두이노 공식 사이트에서 찾아봄
            > url : ``https://www.arduino.cc/en/Guide/ArduinoUno#install-the-board-drivers``

            - 방법
                1. 똑같이 기타장치 > USB Serial 화면까지 진입
                2. 우클릭 후 드라이버 업데이트
                3. 내 컴퓨터에서 드라이버 찾아보기
                4. 경로 입력하는거 말고 하단에 드라이버 목록에서 직접 선택 클릭
                5. 디스크 있음 클릭
                6. 복사항 제조업체 파일 위치를 다음 파일 경로로 설정
                    - ``C:\Program Files (x86)\Arduino\drivers\arduino.inf``
                7. 다음 진행하여 설치
                8. 장치 및 포트 인식 확인#   a r d u i n o _ e x e r  
 