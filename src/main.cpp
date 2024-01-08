#include "color.h"
#include "types.h"
#include "vec2.h"
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>

typedef std::function<void(color *pixel, int x, int y, int image_width,
                           int image_height)>
    component;

#define MAX_LAYERS 10

bool is_in_rounded_square(int x, int y, decimal_t a, decimal_t b, decimal_t c) {
  return pow(fmax(abs(x) - a, 0), 2) + pow(fmax(abs(y) - b, 0), 2) <=
         pow(c, 2) + 0.2;
}

bool is_in_pill(int x, int y) {
  return is_in_rounded_square(x, y, 231, 0, 7.5);
}

bool is_in_superellipse(int x, int y, int a, int b, int n) {
  return std::pow(std::abs(x) / static_cast<double>(a), n) +
             std::pow(std::abs(y) / static_cast<double>(b), n) <=
         1.1;
}

bool is_in_squircle(int x, int y, int size) {
  return is_in_superellipse(x, y, size / 2, size / 2, 5);
}

bool is_in_circle(int x, int y, int cx, int cy, int radius) {
  return pow(x - cx, 2) + pow(y - cy, 2) <= pow(radius, 2);
}

struct App {
  color color;
  std::string name;
};

#define POINTS 100
#define NOISE_WIDTH 80
#define NOISE_HEIGHT 80
#define NOISE_LEN NOISE_WIDTH *NOISE_HEIGHT

struct Point {
  point2 point;
  int radius;
  decimal_t brightness;
};

void generate_noise(decimal_t *noise, unsigned int seed) {
  std::srand(seed);

  Point points[POINTS]{};
  for (int i = 0; i < POINTS; i++) {
    points[i] = {.point = point2(rand() % NOISE_WIDTH, rand() % NOISE_HEIGHT),
                 .radius = rand() % 10,
                 .brightness = static_cast<decimal_t>(rand() % 10) / 10};
  }

  for (int x = 0; x < NOISE_WIDTH; x++) {
    for (int y = 0; y < NOISE_HEIGHT; y++) {
      for (int i = 0; i < POINTS; i++) {
        // highly inefficient, replace with math
        auto p = points[i];
        if (is_in_circle(x, y, p.point.x(), p.point.y(), p.radius)) {
          noise[x + (y * NOISE_WIDTH)] = p.brightness;
        }
      }
    }
  }
}

// \min\left(\max\left(\left(\sin\left(\frac{y-1000}{350}\right)\cdot300\right)\cdot\frac{s\left(y\right)}{900}\
// +200,-\frac{w}{2}+p\right),\frac{w}{2}-p\right)=x-\frac{w}{2}
// s\left(y\right)=\frac{\left(y-\frac{h}{2}\right)^{2}\ \cdot2}{h}+1000\
// h=2796
// w=1290
// p=104

void pill_component(color *pixel, int x, int y, int image_width,
                    int image_height) {
  auto pill_height = 15.0;
  auto pill_padding_bottom = 24.0;
  auto pill_width = 462.0;

  auto pill_middle_x = image_width / 2;
  auto pill_middle_y = image_height - pill_padding_bottom - pill_height;

  if (is_in_rounded_square(x - pill_middle_x, y - pill_middle_y,
                           pill_width / 2 - pill_height / 2, 0,
                           pill_height / 2)) {
    *pixel = color(1, 1, 1, 1);
  }
}

component create_app_component(int image_width, int image_height) {

  auto padding_x = 53;
  auto padding_y = 282;

  auto padding_app = 52;
  auto width = image_width - (padding_x * 2);
  auto height = image_height - (padding_y * 2);

  auto app_size = 192;
  auto app_container_size = app_size + (padding_app * 2);

  auto apps_x = width / app_container_size;
  auto apps_y = height / app_container_size;

  auto max_apps = apps_x * apps_y;
  // std::clog << "max_apps" << max_apps << '\n';

  auto app_end = app_container_size - padding_app;
  auto component = [padding_y, padding_x, app_container_size, apps_x, max_apps,
                    padding_app, app_end,
                    app_size](color *pixel, int x, int y, int image_width,
                              int image_height) {
    if (y > padding_y && y < (image_height - padding_y) && x > padding_x &&
        x < (image_width - padding_x)) {
      auto x_apps = x - padding_x;
      auto x_app = x_apps % app_container_size;

      auto y_apps = y - padding_y;
      auto y_app = y_apps % app_container_size;

      auto i_app = ((x_apps / app_container_size)) +
                   (((y_apps / app_container_size)) * apps_x);

      // std::clog << "i_app" << i_app << '\n';

      if (i_app < max_apps) {
        // app padding
        if (y_app >= padding_app && y_app < app_end && x_app >= padding_app &&
            x_app < app_end) {
          // matrix[y][x] = color(1, 0, 0, 1);
          int squircleCenter = app_container_size / 2;
          if (is_in_squircle(x_app - squircleCenter, y_app - squircleCenter,
                             app_size)) {
            // static_cast<decimal_t>(i_app) / (apps_x * apps_y)
            *pixel = color(
                1 - static_cast<decimal_t>(x_app - padding_app) / app_size,
                1 - static_cast<decimal_t>(y_app - padding_app) / app_size, 0,
                1);
          }
        }
      }
    }
  };
  return component;
}

component create_background_component(decimal_t *noise) {
  return [noise](color *pixel, int x, int y, int image_width,
                 int image_height) {
    auto noise_val =
        noise[(x % NOISE_WIDTH + ((y % NOISE_HEIGHT) * NOISE_WIDTH))];
    decimal_t brightness{};

    auto func_pos =
        (sin(static_cast<decimal_t>(image_height - y - 1000) / 350.0) * 300.0) *
            ((static_cast<decimal_t>(
                  std::pow(image_height - y - image_height / 2, 2)) *
              2.0) /
                 image_height +
             1000) /
            900.0 +
        200 + image_width / 2.0;
    auto distance = ((x - func_pos) / image_width);
    auto percent = static_cast<decimal_t>(x) / func_pos;

    if (distance > 0) {
      // left side
      // if (sin((y - image_height / 3) / 515.0) * 460 >
      //             x - image_width / 2) {

      // if (distance > 1) {
      // std::clog << "x:" << x << "func_pos:" << func_pos << '\n';
      // }
      // brightness = abs(((x - func_pos) / image_width));
      // fmax(noise_val - 0.5, 0));

      brightness = 0.5 - ((sin((1.0 / (static_cast<decimal_t>(std::pow(
                                           (14 * distance + 0.05), 2)) +
                                       0.118)))) *
                          (1.0 / (percent + 0.8))) /
                             2;
    } else {
      // right side
      brightness =
          10 *
          pow(1 - abs(percent) -
                  (0.28 + 0.3 * (static_cast<decimal_t>(y) / image_height)),
              6);
      // brightness = abs(sin((x - func_pos) / image_width));
    }

    if (brightness > 1) {
      brightness = 1;
    } else if (brightness < 0) {
      brightness = 0;
    }

    // if (abs(distance) > 1.0) {
    //   std::clog << "distance:" << distance << "percent:" << percent << '\n';
    // }
    *pixel =
        color((static_cast<decimal_t>(x) / image_width) * brightness,
              (static_cast<decimal_t>(y) / image_height) * brightness, 0, 1);
  };
}

void render(color **matrix, int image_width, int image_height,
            component *components[], int component_len,
            decimal_t slide_up_percent) {
  int start_height = static_cast<decimal_t>(image_height) * slide_up_percent;
  for (int y = start_height; y < image_height; ++y) {
    for (int x = 0; x < image_width; ++x) {

      color *pixel = &matrix[y - start_height][x];

      for (int i = 0; i < component_len; i++) {
        (*components[i])(pixel, x, y, image_width, image_height);
      }
    }
  }
}

int main() {
  decimal_t noise[NOISE_LEN]{};

  generate_noise(noise, 1000);

  // Image

  // int aspect_ratio = 19.5 / 9;
  int image_height = 2796; // full 2796
  int image_width = 1290;  // full 1290

  // // Calculate the image height, and ensure that it's at least 1.
  // int image_width = static_cast<int>(image_height / aspect_ratio);
  // image_width = (image_width < 1) ? 1 : image_width;

  // Layer->Y->Z->color
  color ***layers = new color **[MAX_LAYERS];

  for (int layer = 0; layer < MAX_LAYERS; layer++) {
    layers[layer] = new color *[image_height];
    for (int i = 0; i < image_height; i++) {
      layers[layer][i] = new color[image_width]();
      // for (int j = 0; j < image_width; j++) {
      //   layers[layer][i][j] = color();
      // }
    }
  }

  for (int i = 0; i < image_height; ++i) {
    for (int j = 0; j < image_width; ++j) {
      layers[0][i][j] = color(0.0, 0.0, 0.0, 1);
    }
  }

  auto bg_component = create_background_component(noise);
  component p_component = pill_component;
  auto app_component = create_app_component(image_width, image_height);

  component *components[]{&bg_component, &p_component};
  // render_apps(layers[1], image_width, image_height);

  component *layer1_components[]{&app_component};
  render(layers[1], image_width, image_height, layer1_components, 1, 0.1);
  render(layers[2], image_width, image_height, components, 2, 0.4);
  // render_pill(layers[2], image_width, image_height);

  // for (int i = 0; i < image_height; ++i) {
  //   for (int j = 0; j < image_width; ++j) {
  //     layers[1][i][j] = color(0, decimal_t(j) / (image_height - 1), 0,
  //                             decimal_t(i) / (image_width - 1));
  //   }
  // }
  //

  // // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int i = 0; i < image_height; ++i) {
    std::clog << "\rScanlines remaining: " << (image_height - i) << ' '
              << std::flush;
    for (int j = 0; j < image_width; ++j) {
      auto final_c = color();

      for (int l = 0; l < MAX_LAYERS; ++l) {
        auto c = layers[l][i][j];
        if (c.a() > 0.0) {
          final_c = c;
          if (final_c.r() > 1) {
            final_c.e[0] = 1;
          }
          if (final_c.g() > 1) {
            final_c.e[1] = 1;
          }
          if (final_c.b() > 1) {
            final_c.e[2] = 1;
          }
          if (final_c.a() > 1) {
            final_c.e[3] = 1;
          }
        }
      }

      write_color(std::cout, final_c);
    }
  }

  std::clog << "\rDone.                 \n";

  // for (int layer = 0; layer < MAX_LAYERS; layer++) {
  //   delete layers[layer];
  // }
}
