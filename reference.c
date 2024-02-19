#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef float num;
static const num BOUNDED_LIMIT = 4;

num magnitude(num real, num imag) {
    return real * real + imag * imag;
}

num squared_re(num real, num imag) {
    return real * real - imag * imag;
}

num squared_im(num real, num imag) {
    return 2 * real * imag;
}

void draw_to_terminal(size_t width, size_t height, const num* lattice_re, const num* lattice_im) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            num r = lattice_re[x + y * width];
            num m = lattice_im[x + y * width];
            if (magnitude(r, m) < BOUNDED_LIMIT) {
                printf("█");
            } else {
                printf("▒");
            }
                //printf("%10.5f+%10.5fi  |c|=%10.5f\n", r, m, magnitude(r, m));
        }
        puts("");
    }
}

void clear_terminal() {
    char* clear = "\033[2J";
    char* to_top = "\033[H";
    printf("%s%s", clear, to_top);
}

int main(int argc, char* argv[]) {
    size_t width = argc >= 3 ? strtol(argv[1], NULL, 0) : 81;
    size_t height = argc >= 3 ? strtol(argv[2], NULL, 0) : 41;
    size_t max_iterations = argc >= 4 ? strtol(argv[3], NULL, 0) : 1063;

    size_t size = width * height;
    num* lattice_re = calloc(size, sizeof(num));
    num* lattice_im = calloc(size, sizeof(num));
    num* c_re = calloc(size, sizeof(num));
    num* c_im = calloc(size, sizeof(num));
    int animation = 0;

    num min_re = -2, max_re = 0.5, min_im = -1, max_im = 1;
    // y is imaginary, x is real
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            //printf("%5d %5d %5d %5d %5d %5d\n", x, y, x + y * width, width, height, width * height);
            c_re[x + y * width] = (max_re - min_re) * x / (width - 1) + min_re;
            c_im[x + y * width] = (max_im - min_im) * y / (height - 1) + min_im;
        }
        // printf("%f %f   %f %f\n", c_re[y * height], c_im[y * height], c_re[width - 1 + y * height], c_im[width - 1 + y * height]);
    }

    size_t iteration = 0;
    while (iteration < max_iterations) {
        for (size_t i = 0; i < size; i++) {
            num new_re = squared_re(lattice_re[i], lattice_im[i]) + c_re[i];
            num new_im = squared_im(lattice_re[i], lattice_im[i]) + c_im[i];
            lattice_re[i] = new_re;
            lattice_im[i] = new_im;
        }
        
        if (iteration % 50 && animation) {
            draw_to_terminal(width, height, lattice_re, lattice_im);
            usleep(1000 * 100); // 100 milliseconds
            if (max_iterations > 2) clear_terminal();
        }
        iteration++;
    }
    draw_to_terminal(width, height, lattice_re, lattice_im);
    
    free(lattice_re);
    free(lattice_im);
    free(c_re);
    free(c_im);
    return 0;
}
