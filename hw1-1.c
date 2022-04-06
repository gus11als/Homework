#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// github test3

int main() {
    char d[9]; //00101110
    char p[12];
    printf("�ع��ڵ�� ��ȯ �Է�\n");

    scanf("%s", d);

    printf("�Է��� ������: %s\n", d);


    p[0] = d[0] ^ d[1] ^ d[3] ^ d[4] ^ d[6];
    p[1] = d[0] ^ d[2] ^ d[3] ^ d[5] ^ d[6];
    p[2] = d[0];
    p[3] = d[1] ^ d[2] ^ d[3] ^ d[7];
    p[4] = d[1];
    p[5] = d[2];
    p[6] = d[3];
    p[7] = d[4] ^ d[5] ^ d[6] ^ d[7];
    p[8] = d[4];
    p[9] = d[5];
    p[10] = d[6];
    p[11] = d[7];

    printf("�ع��ڵ�: %c", d[0] ^ d[1] ^ d[3] ^ d[4] ^ d[6]);
    printf("%c", d[0] ^ d[2] ^ d[3] ^ d[5] ^ d[6]);
    printf("%c", p[2]);
    printf("%d", d[1] ^ d[2] ^ d[3] ^ d[7]);
    printf("%c", p[4]);
    printf("%c", p[5]);
    printf("%c", p[6]);
    printf("%d", d[4] ^ d[5] ^ d[6] ^ d[7]);
    printf("%c", p[8]);
    printf("%c", p[9]);
    printf("%c", p[10]);
    printf("%c", p[11]);


    printf("\n");

}