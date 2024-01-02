#include "color.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>

// class camera {
// public:
//   decimal_t focal_length, viewport_height, viewport_width;
//   point3 camera_center;
//   vec3 viewport_u, viewport_v, viewport_upper_left, pixel00_loc;
//   int pixel_delta_u, pixel_delta_v;
//
//   camera(decimal_t focal_length, decimal_t viewport_height,
//          decimal_t aspect_ratio, point3 camera_center)
//       : focal_length{focal_length}, viewport_height{viewport_height},
//         viewport_width{viewport_height * aspect_ratio},
//         camera_center{camera_center}, viewport_u{vec3(viewport_width, 0, 0)},
//         viewport_v{vec3(0, -viewport_height, 0)},
//         pixel_delta_u{viewport_u / image_width},
//         pixel_delta_v{viewport_v / image_height} {};
// };
// auto main_camera = camera(
//     2.0, 2.0, static_cast<decimal_t>(image_width) / image_height, point3());

// check which t offset of a ray falls on the circle
// x^2+y^2+z^2=r^2
// (x-Cx)^2+(y-Cy)^2+(z-Cz)^2=r^2
// (P - C)^2 = r^2 -- all points on circle
// ray_point(t) = origin + t * direction
// ray_point(t) = O + t * D
// P is the ray_point
// ((O + t * D) - C)^2 = r^2
// (O + tD)^2 - 2*(O + tD)*C + C^2
// O^2 + 2OtD + tD^2 - 2CO + 2CtD + C^2
// ... we need to do some further transformations to create an equation which
// gives us the value t we need for the point to be on the circle

decimal_t hit_sphere(const point3 &center, decimal_t radius, const ray &r) {
  vec3 oc = r.origin() - center;
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0 * dot(oc, r.direction());
  auto c = dot(oc, oc) - radius * radius;
  auto discriminant = b * b - 4 * a * c;
  return discriminant;
}

color ray_color(const ray &r) {
  auto discriminant = hit_sphere(point3(0, 0, -0.7), 0.5, r);
  if (discriminant >= 0) {
    return color(discriminant, 0, 0);
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5 * (unit_direction.x() + 1.0);
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

// int main() {
//
//   // Image
//
//   auto aspect_ratio = 16.0 / 9.0;
//   int image_width = 400;
//
//   int image_height = image_width / aspect_ratio;
//   image_height = (image_height < 1) ? 1 : image_height;
//
//   // Camera
//
//   auto focal_length = 1.0;
//   auto viewport_height = 2.0;
//   auto viewport_width =
//       viewport_height * (static_cast<double>(image_width) / image_height);
//   auto camera_center = point3(0, 0, 0);
//
//   // Calculate the vectors across the horizontal and down the vertical
//   viewport
//   // edges.
//   auto viewport_u = vec3(viewport_width, 0, 0);
//   auto viewport_v = vec3(0, -viewport_height, 0);
//
//   // Calculate the horizontal and vertical delta vectors from pixel to pixel.
//   auto pixel_delta_u = viewport_u / image_width;
//   auto pixel_delta_v = viewport_v / image_height;
//
//   // pixel rays would intersect at focal length?
//   // Calculate the location of the upper left pixel.
//   auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
//                              viewport_u / 2 - viewport_v / 2;
//   auto pixel00_loc =
//       viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
//
//   // Render
//
//   std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
//
//   for (int j = 0; j < image_height; ++j) {
//     std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
//               << std::flush;
//     for (int i = 0; i < image_width; ++i) {
//       auto pixel_center =
//           pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
//       auto ray_direction = pixel_center - camera_center;
//       ray r(camera_center, ray_direction);
//
//       color pixel_color = ray_color(r);
//       write_color(std::cout, pixel_color);
//     }
//   }
//   std::clog << "\rDone.                 \n";
// }
