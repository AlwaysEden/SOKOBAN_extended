#include <stdio.h>
#include <conio.h>
#include <time.h>

int main() {
    int key;
    clock_t start_time, end_time;

    printf("Press 'F' key and hold it for 2 seconds...\n");

    // 기다릴 시간 설정 (2초)
    int wait_time = 2 * CLOCKS_PER_SEC;

    // F 키가 눌러질 때까지 대기
    while (!_kbhit()) {
    }

    // F 키가 눌러진 시간 기록
    start_time = clock();

    // F 키를 누르고 있는 동안 대기
    do {
        // F 키가 눌러져 있는 동안 아무 작업도 수행하지 않음
    } while (clock() - start_time < wait_time && _kbhit());

    // F 키를 뗀 시간 기록
    end_time = clock();

    // 2초 동안 F 키가 눌러져 있었는지 확인
    if (end_time - start_time >= wait_time) {
        printf("F key was held down for 2 seconds.\n");
        // 여기에서 특정 기능을 실행
    } else {
        printf("F key was not held down for 2 seconds.\n");
    }

    return 0;
}

