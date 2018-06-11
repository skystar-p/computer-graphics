# 컴퓨터 그래픽스 HW5 레포트
> 2016-11076 박재현


## 환경
* Arch Linux 4.16.13-2-ARCH
* glm 0.9.9.0-1 (libglm-dev on Ubuntu)
* libpng 1.6.34-2
* 위의 라이브러리들이 설치된 Ubuntu 16.04 LTS에서 작동을 확인하였습니다.

## 이미지 렌더링 하는 방법
```
make run
```

## 구현 내용

* 최종 이미지는 `result.png`로 보실 수 있습니다.
* 여러 개의 Sphere들을 그렸습니다.
* 여러 개의 Polygon들을 그렸습니다.
* Recursive reflection, Recursive refraction을 구현했습니다.
    - 가운데 있는 구와 위에 나란히 선 작은 구들에서 관찰할 수 있습니다.
    - 또한 뒤의 벽과 바닥이 거울면이기 때문에 그곳들에서도 recursive reflection을 관찰할 수 있습니다.
* Phong illumination을 구현했습니다.
    - 각각의 Object들에 ambient, diffuse, specular 값들을 부여하고 그에 맞게 광원에 따라서 색깔을 결정했습니다.
* Obj 파일에서 Polygon을 불러오는 것을 구현했습니다.
    - 소스는 `parse.h`, `parse.cpp`에서 보실 수 있습니다.
    - 사용한 OBJ 파일은 `./assets/octahedron.obj`입니다.
* Ray tracing한 결과를 png 이미지 형식으로 export하는 기능을 구현했습니다.
    - libpng를 사용해서 각 픽셀들의 값을 저장했습니다.
* Texture mapping과 Bump mapping을 구현했습니다.
    - 왼쪽의 큰 구에서 Texture mapping과 Bump mapping을 관찰할 수 있습니다.
    - 사용한 텍스쳐 파일들은 `./assets/texture.png`, `./assets/normals.png`입니다.
* Distributed Ray Tracing을 구현했습니다.
    - 한 픽셀에 기여하는 Ray를 여러 개를 사용해서 렌더링했습니다.
    - 구현은 `world.cpp`의 `render` 함수에서 보실 수 있습니다.
* Multi-threading으로 렌더링 속도를 빠르게 했습니다.
