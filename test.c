#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shuffleImages(int* imageOrder, int numImages) {
    // 初始化隨機種子
    srand(time(NULL));

    // 將圖片索引填充到數組中，確保每個圖片索引在 imageOrder 數組中出現的次數與其在 images 數組中的數量相同
    int imageCounts[] = {6, 12, 9, 1}; // images[0]～images[3]各自的圖片數量
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < imageCounts[i]; ++j) {
            imageOrder[index++] = i;
        }
    }

    // 隨機交換數組中的元素
    for (int i = numImages - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = imageOrder[i];
        imageOrder[i] = imageOrder[j];
        imageOrder[j] = temp;
    }
}

int main() {
    const int numImages = 28; // 總共的圖片數量
    int imageOrder[numImages]; // 存儲圖片的隨機排列順序

    // 將圖片索引打印出來，以便檢查
    printf("Original image order:\n");
    for (int i = 0; i < numImages; ++i) {
        printf("%d ", imageOrder[i]);
    }
    printf("\n");

    // 隨機排列圖片索引
    shuffleImages(imageOrder, numImages);

    // 將隨機排列後的圖片索引打印出來
    printf("\nShuffled image order:\n");
    for (int i = 0; i < numImages; ++i) {
        printf("%d ", imageOrder[i]);
    }
    printf("\n");

    return 0;
}
