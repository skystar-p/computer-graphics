# 컴퓨터 그래픽스 HW3 레포트
> 2016-11076 박재현


## 환경
* Arch Linux 4.15.10-1-ARCH
* freeglut 3.0.0-2
* glm 0.9.8.5-2 (libglm-dev on Ubuntu)

## 실행하는 법
```
make
./hw3.out [filename]
```

## 구현 내용

* 기본적으로 HW2의 3D viewer 기능이 있습니다.
    * 마우스 왼쪽 클릭을 한 채로 움직이면 rotate가 됩니다.
    * 마우스 오른쪽 클릭을 한 채로 움직이면 translate가 됩니다.
    * 키보드 z키를 누르면 Zoom 모드가 됩니다.
    * 키보드 x키를 누르면 Dolly 모드가 됩니다.
    * 마우스 스크롤로 Zoom 또는 Dolly in, out을 할 수 있습니다.
    * 키보드 s키를 누르면 Show all가 됩니다.
* File parsing을 구현했습니다.
    * filename이 지정되지 않으면 기본적으로 `chess.in`을 불러옵니다.
* 하나의 section에 대해서 B-spline과 Catmull-Rom spline으로 closed curve를 구성했습니다.
    * 각 section에 scale, rotation, translation을 적용하였습니다.
    * 각 factor들에 대한 spline들 또한 계산하고, 내부의 subsection들을 나누어서 interpolate 하였습니다.
* 사각형을 이용해서 mesh를 구성했습니다.
* 저의 개인 구현은 `chess.in`에 구현되어 있습니다.
