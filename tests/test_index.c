#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tests.h"

struct tg_point *rand_points(struct tg_rect rect, int npoints) {
    struct tg_point *points = malloc(npoints*sizeof(struct tg_point));
    assert(points);
    for (int i = 0; i < npoints; i++) {
        points[i] = rand_point(rect);
    }
    return points;
}

void test_index_pip_concave() {
    tg_env_set_index(TG_NATURAL);
    int N = 10000;
    struct tg_ring *ring_none = RING_NONE(az);
    struct tg_ring *ring_nat = RING_NATURAL(az);
    struct tg_ring *ring_ystr = RING_YSTRIPES(az);
    int hits_nat = 0;
    int hits_ystr = 0;
    int hits_none = 0;
    struct tg_point *points = rand_points(tg_ring_rect(ring_nat), N);
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_nat, points[i], true).hit) {
            hits_nat++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_ystr, points[i], true).hit) {
            hits_ystr++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_none, points[i], true).hit) {
            hits_none++;
        }
    }
    
    assert(hits_nat == hits_ystr);
    assert(hits_nat == hits_none);
    free(points);

    // struct tg_ring *ring_none = RING_NONE(tx);
    
    assert(tg_ring_index_num_levels(NULL) == 0);
    assert(tg_ring_index_num_levels(ring_none) == 0);
    assert(tg_ring_index_num_levels(ring_nat) > 0);

    assert(tg_ring_index_level_num_rects(NULL, 0) == 0);
    assert(tg_ring_index_level_num_rects(ring_none, 0) == 0);
    assert(tg_ring_index_level_num_rects(ring_nat, 0) > 0);
    assert(tg_ring_index_level_num_rects(ring_nat, -1) == 0);
    
    assert(recteq(tg_ring_index_level_rect(NULL, 0, 0), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_none, 0, 0), R(0,0,0,0)));
    assert(!recteq(tg_ring_index_level_rect(ring_nat, 0, 0), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_nat, 0, -1), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_nat, -1, 0), R(0,0,0,0)));
}

void test_index_pip_circle() {
    tg_env_set_index(TG_NATURAL);
    int N = 10000;
    struct tg_ring *ring_none = CIRCLE_NONE(((struct tg_point){ -112, 33 }), 5, 2000);
    struct tg_ring *ring_nat = CIRCLE_NATURAL(((struct tg_point){ -112, 33 }), 5, 2000);
    struct tg_ring *ring_ystr = CIRCLE_YSTRIPES(((struct tg_point){ -112, 33 }), 5, 2000);
    int hits_nat = 0;
    int hits_ystr = 0;
    int hits_none = 0;
    struct tg_rect rect = tg_ring_rect(ring_nat);
    struct tg_point *points = malloc(N*sizeof(struct tg_point));
    assert(points);
    for (int i = 0; i < N; i++) {
        points[i] = rand_point(rect);
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_nat, points[i], true).hit) {
            hits_nat++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_ystr, points[i], true).hit) {
            hits_ystr++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_contains_point(ring_none, points[i], true).hit) {
            hits_none++;
        }
    }
    
    assert(hits_nat == hits_ystr);
    assert(hits_nat == hits_none);
    free(points);

    // struct tg_ring *ring_none = RING_NONE(tx);
    
    assert(tg_ring_index_num_levels(NULL) == 0);
    assert(tg_ring_index_num_levels(ring_none) == 0);
    assert(tg_ring_index_num_levels(ring_nat) > 0);

    assert(tg_ring_index_level_num_rects(NULL, 0) == 0);
    assert(tg_ring_index_level_num_rects(ring_none, 0) == 0);
    assert(tg_ring_index_level_num_rects(ring_nat, 0) > 0);
    assert(tg_ring_index_level_num_rects(ring_nat, -1) == 0);
    
    assert(recteq(tg_ring_index_level_rect(NULL, 0, 0), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_none, 0, 0), R(0,0,0,0)));
    assert(!recteq(tg_ring_index_level_rect(ring_nat, 0, 0), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_nat, 0, -1), R(0,0,0,0)));
    assert(recteq(tg_ring_index_level_rect(ring_nat, -1, 0), R(0,0,0,0)));
}


void test_index_segments() {
    int N = 100000;
    struct tg_ring *ring_nat = RING_NATURAL(az);
    struct tg_ring *ring_ystr = RING_YSTRIPES(az);
    int hits_nat = 0;
    int hits_ystr = 0;
    struct tg_rect rect = tg_ring_rect(ring_nat);
    struct tg_segment *segs = malloc(N*sizeof(struct tg_segment));
    assert(segs);
    for (int i = 0; i < N; i++) {
        segs[i] = rand_segment(rect);
    }
    for (int i = 0; i < N; i++) {
        if (tg_ring_intersects_segment(ring_nat, segs[i], true)) hits_nat++;
    }
    for (int i = 0; i < N; i++) {
        struct tg_point p = rand_point(rect);
        if (tg_ring_intersects_segment(ring_ystr, segs[i], true)) hits_ystr++;
    }
    assert(hits_nat == hits_ystr);
    free(segs);
}

int tg_ring_index2_num_levels(const struct tg_ring *ring);
int tg_ring_index2_level_num_rects(const struct tg_ring *ring, int levelidx);
struct tg_rect tg_ring_index2_level_rect(const struct tg_ring *ring, int levelidx, int rectidx);

// char *bgcolor = "#075aa8";
double swidth = 2.0;
char *bgcolor = "white";
char *strokes[] = {"#6b7c90", "#a32122", "#279950", "#cccc00", "purple"};
// // char *strokes[] = { "#419d78", "#e0a458", "#ffdbb5", "#c04abc" };
// // char *strokes[] = { "#04e762", "#f5b700", "#dc0073", "#89fc00" };
// char *strokes[] = { "#fff" }; //#A3BD0F", "#A3BD0F", "#A3BD0F", "#A3BD0F" };

enum svg_opts {
    SHOW_SEGMENTS = 1,
    SHOW_INDEX = 2,
};

void write_index_svgs(const struct tg_ring *ring, char *path, enum svg_opts opts) {

    struct tg_rect rrect = tg_ring_rect(ring);


    // Use the tg_line_index_* because they are lightweight wrappers around
    // tg_ring_index_* and this will ensure both work at the same time.
    const struct tg_line *line = (struct tg_line*)ring;

    double pad = 30.0;
    double w = 800.0;
    double h = 800.0;

    // double swidth = 1.5; // stroke width

    int nstrokes = sizeof(strokes)/sizeof(char*);

    FILE *f = fopen(path, "wb+");
    assert(f);

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
               " width=\"%.0f\" height=\"%.0f\" style=\"background-color:%s\">\n", 
               w, h, bgcolor);
    fprintf(f, "<g transform=\"scale(1,-1)\">\n");
    double rw = (rrect.max.x-rrect.min.x);
    double rh = (rrect.max.y-rrect.min.y);

    if ((opts&SHOW_SEGMENTS) == SHOW_SEGMENTS) {
        int npoints = tg_line_num_points(line);

        fprintf(f, "<path d=\"");
        for (int i = 0; i < npoints; i++) {
            struct tg_point p = tg_line_point_at(line, i);
            double x = (((p.x-rrect.min.x)/rw)*(w-pad*2)+pad);
            double y = (((p.y-rrect.min.y)/rh)*(h-pad*2)+pad-h);

            if (i == 0) {
                fprintf(f, "M%f %f ", x, y);
            } else {
                fprintf(f, "L%f %f ", x, y);
            }
        }
        fprintf(f, "\" stroke=\"%s\" stroke-width=\"%f\" "
                "fill-opacity=\"0\" stroke-opacity=\"1\" />\n",
                strokes[0], swidth);
    }
    if ((opts&SHOW_INDEX) == SHOW_INDEX) {
        // draw MBR
        struct tg_rect lrect = rrect;
        int nlevels = tg_line_index_num_levels(line)+1;
        double xmin = (((lrect.min.x-rrect.min.x)/rw)*(w-pad*2)+pad);
        double ymin = (((lrect.min.y-rrect.min.y)/rh)*(h-pad*2)+pad-h);
        double xmax = (((lrect.max.x-rrect.min.x)/rw)*(w-pad*2)+pad);
        double ymax = (((lrect.max.y-rrect.min.y)/rh)*(h-pad*2)+pad-h);
        fprintf(f, "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" "
                "stroke=\"%s\" stroke-width=\"%f\" "
                "fill-opacity=\"0\" stroke-opacity=\"1\"/>\n",
                xmin, ymin, xmax-xmin, ymax-ymin, strokes[nlevels%nstrokes],swidth
        );
        // printf("%s %d\n", path, nlevels);
        for (int i = 0; i < nlevels; i++) {
            int nrects = tg_line_index_level_num_rects(line, i);
            for (int j = 0; j < nrects; j++) {
                struct tg_rect lrect = tg_line_index_level_rect(line, i, j);
                double xmin = (((lrect.min.x-rrect.min.x)/rw)*(w-pad*2)+pad);
                double ymin = (((lrect.min.y-rrect.min.y)/rh)*(h-pad*2)+pad-h);
                double xmax = (((lrect.max.x-rrect.min.x)/rw)*(w-pad*2)+pad);
                double ymax = (((lrect.max.y-rrect.min.y)/rh)*(h-pad*2)+pad-h);
                fprintf(f, "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" "
                        "stroke=\"%s\" stroke-width=\"%f\" "
                        "fill-opacity=\"0\" stroke-opacity=\"1\"/>\n",
                        xmin, ymin, xmax-xmin, ymax-ymin, strokes[(nlevels-(i+1))%nstrokes],
                        swidth
                );
            }
        }
    }
    fprintf(f, "</g>\n");
    fprintf(f, "</svg>\n");
    fclose(f);
}

void test_index_svg() {
    mkdir("svg", 0700);
    char *states[] = { "tx", "az", "ri", "br", "bc" };
    for (size_t i = 0; i < sizeof(states)/sizeof(char*); i++) {
        struct tg_ring *ring_nat = (void*)gc_geom(load_geom(states[i], TG_NATURAL));
        char path[128];
        snprintf(path, sizeof(path), "svg/%s-index.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_INDEX);
        snprintf(path, sizeof(path), "svg/%s-segs.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_SEGMENTS);
        snprintf(path, sizeof(path), "svg/%s-both.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_SEGMENTS|SHOW_INDEX);
    }

    for (size_t i = 0; i < sizeof(states)/sizeof(char*); i++) {
        struct tg_ring *ring_nat = (void*)gc_geom(load_geom_flipped(states[i], TG_NATURAL));
        char path[128];
        snprintf(path, sizeof(path), "svg/flipped-%s-index.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_INDEX);
        snprintf(path, sizeof(path), "svg/flipped-%s-segs.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_SEGMENTS);
        snprintf(path, sizeof(path), "svg/flipped-%s-both.svg", states[i]);
        write_index_svgs(ring_nat, path, SHOW_SEGMENTS|SHOW_INDEX);
    }

    {
        struct tg_ring *ring = CIRCLE(((struct tg_point){ -112, 33 }), 5, 1000);
        write_index_svgs(ring, "svg/circle-index.svg", SHOW_INDEX);
        write_index_svgs(ring, "svg/circle-segs.svg", SHOW_SEGMENTS);
        write_index_svgs(ring, "svg/circle-both.svg", SHOW_SEGMENTS|SHOW_INDEX);
    }
    {
        
        struct tg_ring *ring = RANDOM(1000);
        write_index_svgs(ring, "svg/random-index.svg", SHOW_INDEX);
        write_index_svgs(ring, "svg/random-segs.svg", SHOW_SEGMENTS);
        write_index_svgs(ring, "svg/random-both.svg", SHOW_SEGMENTS|SHOW_INDEX);
    }
    {
        
        struct tg_ring *ring = RANDOM(100);
        write_index_svgs(ring, "svg/random100-index.svg", SHOW_INDEX);
        write_index_svgs(ring, "svg/random100-segs.svg", SHOW_SEGMENTS);
        write_index_svgs(ring, "svg/random100-both.svg", SHOW_SEGMENTS|SHOW_INDEX);
    }
    {
        struct tg_ring *ring = RANDOM(100);
        ring = (struct tg_ring*)gc_geom(flip_geom((struct tg_geom*)ring, TG_NATURAL));
        write_index_svgs(ring, "svg/flipped-random100-index.svg", SHOW_INDEX);
        write_index_svgs(ring, "svg/flipped-random100-segs.svg", SHOW_SEGMENTS);
        write_index_svgs(ring, "svg/flipped-random100-both.svg", SHOW_SEGMENTS|SHOW_INDEX);
    }
    
}

void test_index_ystripes_circle(void) {
    struct tg_ring *ring = tg_circle_new_ix((struct tg_point) { -112, 33 }, 5, 
        10000, TG_YSTRIPES);
    assert(ring);
    int npoints = 10000;
    struct tg_point *points = rand_points(tg_ring_rect(ring), npoints);

    int hits = 0;
    for (int i = 0; i < npoints; i++) {
        if (tg_ring_contains_point(ring, points[i], true).hit) {
            hits++;
        }
    }
    if (hits < 7700 || hits > 8000) {
        fprintf(stderr, "exected between 7700-8000, got: %d\n", hits);
        assert(0);
    }
    free(points);
    tg_ring_free(ring);
}

void test_index_various(void) {
    struct tg_ring *ring = RING_NATURAL(az);
    assert(ring);
    assert(tg_ring_index_spread(ring));
    assert(tg_line_index_spread((struct tg_line*)ring));

    ring = RING_NONE(az);
    assert(ring);
    assert(!tg_ring_index_spread(ring));
    assert(!tg_line_index_spread((struct tg_line*)ring));
}

void test_index_defaults(void) {
    struct tg_ring *ring;
    enum tg_index defix = tg_env_get_default_index();
    int spread = tg_env_get_index_spread();
    tg_env_set_index(99);
    assert(tg_env_get_default_index() == defix);
    tg_env_set_index_spread(1);
    assert(tg_env_get_index_spread() == spread);
    tg_env_set_index_spread(2);
    assert(tg_env_get_index_spread() == 2);
    tg_env_set_index_spread(100);
    assert(tg_env_get_index_spread() == 100);
    tg_env_set_index_spread(4096);
    assert(tg_env_get_index_spread() == 4096);
    tg_env_set_index_spread(4097);
    assert(tg_env_get_index_spread() == 4096);
    tg_env_set_index_spread(48);
    assert(tg_env_get_index_spread() == 48);
    
    tg_env_set_index(TG_NONE);
    tg_env_set_index_spread(49);
    assert(tg_env_get_default_index() == TG_NONE);
    ring = RING(az);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 0);
    assert(tg_env_get_default_index() == TG_NONE);

    tg_env_set_index_spread(67);
    tg_env_set_index(TG_NATURAL);
    ring = RING(az);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 67);
    assert(tg_env_get_default_index() == TG_NATURAL);

    tg_env_set_index_spread(51);
    tg_env_set_index(TG_YSTRIPES);
    assert(tg_env_get_default_index() == TG_YSTRIPES);
    ring = RING(az);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 51);
    assert(tg_env_get_default_index() == TG_YSTRIPES);

    tg_env_set_index(TG_NONE);
    ring = RING_INDEX(tg_index_with_spread(TG_NATURAL, 10), az);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 10);
    tg_ring_free(ring);
    
    ring = RING_INDEX(tg_index_with_spread(TG_NATURAL, 1), az);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 2);
    tg_ring_free(ring);

    enum tg_index ixws, ix0;
    int sp0;

    ixws = tg_index_with_spread(TG_NATURAL, 5000);
    ix0 = tg_index_extract_spread(ixws, &sp0);
    assert(ix0 == TG_NATURAL);
    assert(sp0 == 4096);

    ixws = tg_index_with_spread(TG_YSTRIPES, 0);
    ix0 = tg_index_extract_spread(ixws, &sp0);
    assert(ix0 == TG_YSTRIPES);
    assert(sp0 == 51);

    ring = RING_INDEX(tg_index_with_spread(TG_NATURAL, 5000), tx);
    assert(ring);
    assert(tg_ring_index_spread(ring) == 4096);
    tg_ring_free(ring);

    tg_env_set_index_spread(spread);
    tg_env_set_index(defix);
}

int main(int argc, char **argv) {
    do_test(test_index_pip_concave);
    do_test(test_index_pip_circle);
    do_test(test_index_segments);
    do_test(test_index_ystripes_circle);
    do_test(test_index_svg);
    do_test(test_index_defaults);
    do_test(test_index_various);
    return 0;
}
