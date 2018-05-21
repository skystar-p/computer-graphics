# 컴퓨터 그래픽스 HW4 레포트
> 2016-11076 박재현


## 환경
* Arch Linux 4.15.10-1-ARCH
* freeglut 3.0.0-2
* glm 0.9.8.5-2 (libglm-dev on Ubuntu)

## 실행하는 법
```
make
./hw4.out
```

## 구현 내용

* 기본적으로 HW2의 3D viewer 기능이 있습니다.
    - 마우스 왼쪽 클릭을 한 채로 움직이면 rotate가 됩니다.
    - 마우스 오른쪽 클릭을 한 채로 움직이면 translate가 됩니다.
    - 키보드 z키를 누르면 Zoom 모드가 됩니다.
    - 키보드 x키를 누르면 Dolly 모드가 됩니다.
    - 마우스 스크롤로 Zoom 또는 Dolly in, out을 할 수 있습니다.
    - 키보드 s키를 누르면 Show all가 됩니다.

* 체스 말은 불투명하게 렌더링 했습니다.
    - 각 section들의 normal vector들을 평균을 내서 각 vertex에 주고, normal vector interpolation을 enable해서 부드러운 면을 렌더링 했습니다.
    - 면들의 material 또한 설정해줌으로서 빛을 잘 받아서 잘 그려지는 것을 확인했습니다.

* 반투명한 정육면체를 렌더링 했습니다.
    - Blend를 enable 함으로서 반투명한 면을 그릴 수 있게 하였습니다.
    - 각 면의 material들을 서로 다르게 결정해주었습니다. 각각 ruby, gold, silver, chrome, jade, brass를 나타내었습니다.
    - 각 material들의 ambient, diffuse, specular coefficients는 `http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html` 를 참고했습니다.

* Depth Ordering을 구현했습니다.
    - 반투명한 물체와 불투명한 물체의 겹친 모양을 잘 표현하기 위해서 직접 ordering을 구현했습니다.
    - 불투명한 물체를 먼저 그리고, 반투명한 물체는 거리를 계산해서 맨 뒤에 있는 면부터 그림으로서 적절한 view가 보이도록 만들었습니다.

```c++
// comparator for polygon order
bool operator< (const Polygon& p) {
    glm::vec3 c(camera_pos[0], camera_pos[1], camera_pos[2]);
    glm::vec3 my_center =
        (points[0] + points[1] + points[2] + points[3]) / 4.0f;
    glm::vec3 others_center =
        (p.points[0] + p.points[1] + p.points[2] + p.points[3]) / 4.0f;

    return glm::length(my_center - c) > glm::length(others_center - c);
}

// hopefully we can use std::sort to sort the polygons
std::sort(cube_faces.begin(), cube_faces.end());
```

* Light source들을 여러 개 만들었습니다.
    - OpenGL에서 제공하는 기본 Light source를 네 개를 directional light source로 만들었습니다.
    - 물체를 기준으로 보는 방향에 하나, 오른쪽에 하나, 왼쪽에 하나, 아래쪽에 하나를 두었습니다.
    - Homework 2에 구현한 trackball로 여러 viewpoint에서 관찰할 수 있게 구현했습니다.
    - 숫자 1, 2, 3, 4를 누르면 차례대로 라이트를 켜고 끌 수 있습니다. 기본적으로 1번만 켜져있습니다.
