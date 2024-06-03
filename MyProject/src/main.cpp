#include "UsefulThings.h"

#include "camera.h"
#include "GeometryObjects/hittable_list.h"
#include "Random/BlueNoise.h"
#include "SceneReader.h"
#include "UsefulThings.h"
#include "UserInteraction.h"

namespace fs = std::filesystem;

int main() {

    getSettingsFromUser();

    our_clock.start();

    BlueNoiseGenerate(MAX_SAMPLES + MAX_BOUNCE);

    hittable_list world;
    ReadScene(world);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = IMAGE_WIDTH;
    cam.samples_per_pixel = MAX_SAMPLES;
    cam.max_bounce = MAX_BOUNCE;
    cam.seed = 0;

    {
        std::ifstream camera_data(current_folder + "data.txt");

        camera_data >> cam.vertical_fov;
        double x, y, z;
        camera_data >> x >> y >> z;
        cam.look_from = point3(x, y, z);
        camera_data >> x >> y >> z;
        cam.look_at = point3(x, y, z);
        cam.view_up = vec3(0, 1, 0);

        camera_data.close();
    }

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);
    return 0;
}