#include <bits/stdc++.h>
#include "bmplib.cpp"

using namespace std;
unsigned char image[SIZE][SIZE][RGB];
char imageFileName[100];

int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

enum Color {
    BLACK = 0,
    WHITE = 255
};

void loadImage();

void saveImage();

//------------------------------------------------

void blackWhite();

void invert();

void merge();

void flipImage();

void rotate();

void darkenOrLighten();

void shrink();

void blur();

void detectEdges();

void enlarge();

void shuffle();

void skewHorizontally();

void skewVertically();

//-------------------------------------------------

void systemRun();

char menu();

int main() {
    cout << "Ahlan ya User ya habibi\n";
    loadImage();
    while (true)
        systemRun();
}

//--------------------------------------------------------

void loadImage() {
    cout << "Enter the source image file name: ";
    cin >> imageFileName;

    strcat(imageFileName, ".bmp");
    readRGBBMP(imageFileName, image);
}

void saveImage() {
    cout << "Enter the target image file name: ";
    cin >> imageFileName;
    strcat(imageFileName, ".bmp");
    writeRGBBMP(imageFileName, image);
}

//--------------------------------------------------------

void blackWhite() {
    //looping over RGB values for all the pixels and computing a Grayscale to measure on it
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int sum = 0;
            for (int k = 0; k < RGB; ++k) {
                sum += image[i][j][k];
            }
            sum /= 3;
            for (int k = 0; k < RGB; ++k)
                image[i][j][k] = sum > 115? BLACK : WHITE;
        }
    }
}

void invert() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                image[i][j][k] = 255 - image[i][j][k];
            }
        }
    }
}

void flipImage() {
    cout << "Flip (h)orizontally or (v)ertically ? ";
    char choice;
    cin >> choice;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < (SIZE - 1) / 2; ++j) {
            for (int k = 0; k < RGB; ++k) {
                if (choice == 'h')
                    swap(image[i][j][k], image[i][SIZE - j - 1][k]);
                else
                    swap(image[j][i][k], image[SIZE - j - 1][i][k]);
            }
        }
    }
}

void rotate() {
    cout << "Rotate (90), (180) or (270) degrees?\n";
    int rotate;
    cin >> rotate;
    while (rotate) {
        rotate -= 90;
        unsigned char newImage[SIZE][SIZE][RGB];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                for (int k = 0; k < RGB; k++) {
                    newImage[j][SIZE - i - 1][k] = image[i][j][k];
                }
            }
        }
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                for (int k = 0; k < RGB; k++) {
                    image[i][j][k] = newImage[i][j][k];
                }
            }
        }
    }
}

void detectEdges() {
    //convert the image to grayscale first
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int sum = 0;
            for (int k = 0; k < RGB; ++k) sum += image[i][j][k];
            image[i][j][0] = sum / 3;
        }
    }
    unsigned char temp[SIZE][SIZE][RGB];
    /* for every pixel if it has a large difference any of its 4 neighbors (up, down, left and right)
    then mark it as an edge*/
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            temp[i][j][0] = WHITE;
            // looping on 4 directions using direction array
            for (int k = 0; k < 4; ++k) {
                int nx = i + dx[k], ny = j + dy[k];
                // check that the new index is in range
                if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && abs(image[i][j][0] - image[nx][ny][0]) >= 33)
                    temp[i][j][0] = 0;
            }
        }
    }
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            for (int k = 0; k < RGB; ++k)
                image[i][j][k] = temp[i][j][0];
}

void mirrorImage() {
    cout << "Mirror (l)eft, (r)ight, (u)pper, (d)own side ? ";
    char choice;
    cin >> choice;
    // in any of the 4 cases you copy pixels to the other side
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < (SIZE - 1) / 2; ++j) {
            for (int k = 0; k < RGB; ++k) {
                if (choice == 'l') {
                    image[i][SIZE - j - 1][k] = image[i][j][k];
                } else if (choice == 'r') {
                    image[i][j][k] = image[i][SIZE - j - 1][k];
                } else if (choice == 'u') {
                    image[SIZE - j - 1][i][k] = image[j][i][k];
                } else {
                    image[j][i][k] = image[SIZE - j - 1][i][k];
                }
            }
        }
    }
}

void cropImage() {
    cout << "Please enter x y l w: ";
    int x, y, l, w;
    cin >> x >> y >> l >> w;
    x--, y--;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                // if you are in range leave the image else just
                if (i >= x && i <= x + w && j >= y && j <= y + w) continue;
                image[i][j][k] = WHITE;
            }
        }
    }
}

void enlarge() {
    // loops on the specific quadrant and makes all the uncolored adjacent pixels the same color.
    unsigned char enlarged[SIZE][SIZE][RGB];
    cout << "Which quarter to enlarge 1, 2, 3 or 4?\n";
    int x;
    cin >> x;
    vector<int> posx{0, 1, 1, 0}, posy{1, 0, 1, 0};
    int h1, h2, v1, v2;
    switch (x) {
        case 1:
            h1 = 0, h2 = SIZE / 2, v1 = 0, v2 = SIZE / 2;
            break;
        case 2:
            h1 = 0, h2 = SIZE / 2, v1 = SIZE / 2, v2 = SIZE;
            break;
        case 3:
            h1 = SIZE / 2, h2 = SIZE, v1 = 0, v2 = SIZE / 2;
            break;
        case 4:
            h1 = SIZE / 2, h2 = SIZE, v1 = SIZE / 2, v2 = SIZE;
            break;
    }
    for (int i = h1; i < h2; ++i) {
        for (int j = v1; j < v2; ++j) {
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < RGB; ++l) {
                    enlarged[(i - h1) * 2 + posx[k]][(j - v1) * 2 + posy[k]][l] = image[i][j][l];
                }
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                image[i][j][k] = enlarged[i][j][k];
            }
        }
    }
}

void shuffle() {
    // This function stores quadrants in an image vector with their new given order.
    cout << "New order of quarters ? 4 3 2 1\n";
    vector<int> order(4);
    for (auto &i: order)cin >> i;

    // this is the image vector and the position vectors so we can reorder the image.
    const int half = SIZE / 2;
    unsigned char img[4][SIZE][SIZE][3];
    vector<int>
            x1{0, 0, half, half}
    , x2{half, half, SIZE, SIZE}
    , y1{0, half, 0, half}
    , y2{half, SIZE, half, SIZE};

    for (int k = 0; k < 4; k++) {
        int h1, h2, v1, v2;
        switch (order[k]) {
            case 1:
                h1 = 0, h2 = half, v1 = 0, v2 = half;
                break;
            case 2:
                h1 = 0, h2 = half, v1 = half, v2 = SIZE;
                break;
            case 3:
                h1 = half, h2 = SIZE, v1 = 0, v2 = half;
                break;
            case 4:
                h1 = half, h2 = SIZE, v1 = half, v2 = SIZE;
                break;
        }
        // this part is where each quadrant is stored in the image vector.
        unsigned char quadrant[half][half][3];
        int x = 0, y = 0;
        for (int i = h1; i < h2; ++i, ++x) {
            y = 0;
            for (int j = v1; j < v2; ++j, ++y) {
                for (int l = 0; l < 3; l++) {
                    quadrant[x][y][l] = image[i][j][l];
                }
            }
        }
        for (int i = 0; i < half; i++) {
            for (int j = 0; j < half; j++) {
                for (int l = 0; l < 3; l++) {
                    img[k][i][j][l] = quadrant[i][j][l];
                }
            }
        }
    }
    // here we overwrite the image with the quadrants we saved.
    for (int i = 0; i < 4; i++) {
        int x = 0, y = 0;
        for (int j = x1[i]; j < x2[i]; ++j, ++x) {
            y = 0;
            for (int k = y1[i]; k < y2[i]; ++k, ++y) {
                if (x < half && y < half) {
                    for (int l = 0; l < 3; l++) {
                        image[j][k][l] = img[i][x][y][l];
                    }
                }
            }
        }
    }
}

// Merge two RGB images by averaging their pixel values.
void merge() {
    // reading the second image
    char filename2[100];
    cout << "Enter the filename of the image that you want to merge with: ";
    cin >> filename2;
    unsigned char image2[SIZE][SIZE][RGB];
    strcat(filename2, ".bmp");
    readRGBBMP(filename2, image2);

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                image[i][j][k] = (image[i][j][k] + image2[i][j][k]) / 2;
            }
        }
    }
}

void darkenOrLighten() {
    cout << "(d)arken or (l)ighten the image ? ";
    char option;
    cin >> option;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                if (option == 'l')
                    image[i][j][k] = min(255, int(1.5 * image[i][j][k]));
                else if (option == 'd')
                    image[i][j][k] /= 2;
            }
        }
    }
}

void shrink() {
    cout << "Shrink the image to (1/2), (1/3) or (1/4) of the original image ? ";
    string s;
    cin >> s;

    // Extract the desired shrink ratio from user input
    int ratio = s.back() - '0';
    int newImageSize = (SIZE + (ratio - 1)) / ratio;

    unsigned char newImage[newImageSize][newImageSize][RGB];

    // Copy the pixels from the original image and skip number of pixels based on the desired ratio
    for (int i = 0; i < SIZE; i += ratio)
        for (int j = 0; j < SIZE; j += ratio)
            for (int k = 0; k < RGB; ++k)
                newImage[i / ratio][j / ratio][k] = image[i][j][k];

    // Update the original image with the resized image, filling the remaining area with white
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            for (int k = 0; k < RGB; ++k)
                image[i][j][k] = i >= newImageSize or j >= newImageSize ? WHITE : newImage[i][j][k];
}

void blur() {
    unsigned char newImage[SIZE][SIZE][RGB];
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                int sum = image[i][j][k];
                int count = 1;

                // Iterate over neighboring pixels to calculate the average value of them
                for (int a = 0; a < 8; ++a) {
                    int x = i + dx[a];
                    int y = j + dy[a];
                    // Check if neighboring pixel coordinates are within image boundaries.
                    if (x >= 0 and x < SIZE and y >= 0 and y < SIZE) sum += image[x][y][k], ++count;
                }
                // Store the average pixel value in the new image.
                newImage[i][j][k] = sum / count;
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < RGB; ++k) {
                image[i][j][k] = newImage[i][j][k];
            }
        }
    }
}

// Skew the image horizontally by a specified degree to the right.
void skewHorizontally() {
    cout << "Please enter the degree to skew right: ";
    long double degree;
    cin >> degree;

    long double pi = 3.1415926535897932384626433832795;

    // Convert user-input degree to radians (rad), ensuring it's within 45 degrees.
    long double rad = (degree > 45 ? int(degree) % 45 : degree) * pi / 180;

    // Calculate the amount of whitespace added due to skew
    int whiteSpace = SIZE * tan(rad);
    int newImageSize = SIZE - whiteSpace;

    if (degree == 45) {
        // If the degree is 45, fill the entire image with white pixels
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                for (int k = 0; k < 3; ++k) {
                    image[i][j][k] = (i + j == 255 ? image[i][j][k] : WHITE);
                }
            }
        }
        return;
    }

    unsigned char newImage[SIZE][newImageSize][3];

    // Calculate the ratio between the original image size and the skewed image size
    double ratio = double(SIZE) / double(newImageSize);

    // Copy pixels from the original image to the skewed image using the calculated ratio
    int cnt = 0;
    for (double j = 0; j < SIZE; j += ratio, cnt++) {
        for (int i = 0; i < SIZE and cnt < newImageSize; ++i) {
            for (int k = 0; k < 3; k++) {
                newImage[i][cnt][k] = image[i][int(j)][k];
            }
        }
    }

    // Adjust the skewed image by filling empty spaces with white pixels.
    for (int i = 0; i < SIZE; i++) {
        int empty = (SIZE - i) * tan(rad);
        int count = 0;
        for (int j = 0; j < SIZE; j++) {
            if (j < empty) {
                for (int k = 0; k < 3; k++) image[i][j][k] = WHITE;
            }
            else if (count < newImageSize) {
                for (int k = 0; k < 3; k++)  image[i][j][k] = newImage[i][count][k];
                count++;
            }
            else {
                for (int k = 0; k < 3; k++) image[i][j][k] = WHITE;
            }
        }
    }
}

void skewVertically() {
    cout << "Please enter the degree to skew up: ";
    long double degree;
    cin >> degree;

    long double pi = 3.1415926535897932384626433832795;

    // Convert user-input degree to radians (rad), ensuring it's within 45 degrees.
    long double rad = (degree > 45 ? int(degree) % 45 : degree) * pi / 180;

    // Calculate the amount of whitespace added due to skew
    int whiteSpace = SIZE * tan(rad);
    int newImageSize = SIZE - whiteSpace;

    if (degree == 45) {
        // If the degree is 45, fill the diagonal with the centered pixel
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                for (int k = 0; k < 3; ++k) {
                    image[j][i][k] = (i + j == 255 ? image[j][i][k] : WHITE);
                }
            }
        }
        return;
    }

    unsigned char newImage[newImageSize][SIZE][3];

    // Calculate the ratio between the original image size and the skewed image size
    double ratio = double(SIZE) / double(newImageSize);

    // Copy pixels from the original image to the skewed image using the calculated ratio
    int cnt = 0;
    for (double i = 0; i < SIZE; i += ratio, cnt++) {
        for (int j = 0; j < SIZE and cnt < newImageSize; j++) {
            for (int k = 0; k < 3; ++k) {
                newImage[cnt][j][k] = image[int(i)][j][k];
            }
        }
    }

    // Adjust the skewed image by filling empty spaces with white pixels.
    for (int i = 0; i < SIZE; i++) {
        int empty = (SIZE - i) * tan(rad);
        int count = 0;
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < 3; ++k) {
                if (j < empty) image[j][i][k] = WHITE;
                else if (count < newImageSize) {
                    image[j][i][k] = newImage[count][i][k];
                    count += k == 2;
                }
                else image[j][i][k] = WHITE;
            }
        }
    }
}

//--------------------------------------------
char menu() {
    cout << "Please select a filter to apply or 0 to exit:\n";
    cout << "1 - Black & White filter\n";
    cout << "2 - Invert filter\n";
    cout << "3 - Merge filter\n";
    cout << "4 - Flip Image\n";
    cout << "5 - Rotate Image\n";
    cout << "6 - Darken and Lighten Image\n";
    cout << "7 - Detect Image edges\n";
    cout << "8 - Enlarge Image\n";
    cout << "9 - Shrink Image\n";
    cout << "a - Mirror 1/2 Image\n";
    cout << "b - Shuffle Image\n";
    cout << "c - Blur Image\n";
    cout << "d - Crop Image\n";
    cout << "e - Skew Image Right\n";
    cout << "f - Skew Image Up\n";
    cout << "s - Save the image to a file\n";
    cout << "0 - Exit\n";

    char choice;
    cin >> choice;
    return choice;
}

void systemRun() {
    char choice = menu();
    switch (choice) {
        case '1':
            blackWhite();
            break;
        case '2':
            invert();
            break;
        case '3':
            merge();
            break;
        case '4':
            flipImage();
            break;
        case '5':
            rotate();
            break;
        case '6':
            darkenOrLighten();
            break;
        case '7':
            detectEdges();
            break;
        case '8':
            enlarge();
            break;
        case '9':
            shrink();
            break;
        case 'a':
            mirrorImage();
            break;
        case 'b':
            shuffle();
            break;
        case 'c':
            blur();
            break;
        case 'd':
            cropImage();
            break;
        case 'e':
            skewHorizontally();
            break;
        case 'f':
            skewVertically();
            break;
        case 's':
            saveImage();
            break;
        case '0':
            exit(0);
    }
}
