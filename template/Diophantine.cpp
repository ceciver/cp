struct Diophantine {
    int ext_gcd(int a, int b, int &x, int &y) {
        if (b == 0) {
            x = (a >= 0 ? 1 : -1);
            y = 0;
            return (a >= 0 ? a : -a);
        }
        int x1, y1;
        int g = ext_gcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }

    // solve a*x + b*y = c
    // returns:
    //  - false  -> no solution
    //  - true & g > 0 -> one solution (x0,y0), gcd(a,b)=g
    //  - true & g = 0 -> a=b=c=0, infinite solutions, (x0,y0)=(0,0)
    bool solve_any(int a, int b, int c, int &x0, int &y0, int &g) {
        if (a == 0 && b == 0) {
            if (c != 0) return false;
            x0 = 0; y0 = 0; g = 0;
            return true;
        }
        int aa = (a >= 0 ? a : -a);
        int bb = (b >= 0 ? b : -b);
        g = std::gcd(aa, bb);
        if (c % g != 0) return false;

        int x, y;
        ext_gcd(aa, bb, x, y);
        if (a < 0) x = -x;
        if (b < 0) y = -y;

        int mult = c / g;
        x0 = x * mult;
        y0 = y * mult;
        return true;
    }

    // general solution shift: x += k * (b/g), y -= k * (a/g)
    void shift(int &x, int &y, int a, int b, int g, int k) {
        if (g == 0) return;
        a /= g; b /= g;
        x += k * b;
        y -= k * a;
    }

    int floor_div(int a, int b) { // b > 0
        if (a >= 0) return a / b;
        return - ( ( -a + b - 1 ) / b );
    }

    int ceil_div(int a, int b) { // b > 0
        if (a >= 0) return ( a + b - 1 ) / b;
        return - ( (-a) / b );
    }

    // x = x0 + k*dx in [lx,rx]
    bool k_range_x(int x0, int dx, int lx, int rx, int &kmin, int &kmax) {
        if (lx > rx) return false;
        const int INF_K = (int)4e18;
        if (dx == 0) {
            if (x0 < lx || x0 > rx) return false;
            kmin = -INF_K;
            kmax =  INF_K;
            return true;
        }
        if (dx > 0) {
            kmin = ceil_div(lx - x0, dx);
            kmax = floor_div(rx - x0, dx);
        } else {
            int D = -dx;
            kmin = ceil_div(x0 - rx, D);
            kmax = floor_div(x0 - lx, D);
        }
        return kmin <= kmax;
    }

    // y = y0 - k*dy in [ly,ry], with dy = a/g
    bool k_range_y(int y0, int dy, int ly, int ry, int &kmin, int &kmax) {
        if (ly > ry) return false;
        const int INF_K = (int)4e18;
        if (dy == 0) {
            if (y0 < ly || y0 > ry) return false;
            kmin = -INF_K;
            kmax =  INF_K;
            return true;
        }
        if (dy > 0) {
            int l = y0 - ry;
            int r = y0 - ly;
            kmin = ceil_div(l, dy);
            kmax = floor_div(r, dy);
        } else {
            int D = -dy;
            int l = ly - y0;
            int r = ry - y0;
            kmin = ceil_div(l, D);
            kmax = floor_div(r, D);
        }
        return kmin <= kmax;
    }

    // count integer (x,y) with:
    //   a*x + b*y = c
    //   x in [minx,maxx], y in [miny,maxy]
    int count_box(int a, int b, int c,
                         int minx, int maxx,
                         int miny, int maxy) {
        if (minx > maxx || miny > maxy) return 0;

        // 0*x + 0*y = c
        if (a == 0 && b == 0) {
            if (c != 0) return 0;
            int w = maxx - minx + 1;
            int h = maxy - miny + 1;
            if (w <= 0 || h <= 0) return 0;
            return w * h;
        }

        int x0, y0, g;
        if (!solve_any(a, b, c, x0, y0, g)) return 0;
        if (g == 0) return 0; // already handled above

        a /= g; b /= g;
        int dx = b;
        int dy = a;

        int kx_min, kx_max, ky_min, ky_max;
        if (!k_range_x(x0, dx, minx, maxx, kx_min, kx_max)) return 0;
        if (!k_range_y(y0, dy, miny, maxy, ky_min, ky_max)) return 0;

        int kmin = max(kx_min, ky_min);
        int kmax = min(kx_max, ky_max);
        if (kmin > kmax) return 0;
        return kmax - kmin + 1;
    }

    // find one (x,y) in the box (if exists)
    bool find_box(int a, int b, int c,
                         int minx, int maxx,
                         int miny, int maxy,
                         int &x, int &y) {
        if (minx > maxx || miny > maxy) return false;

        if (a == 0 && b == 0) {
            if (c != 0) return false;
            x = minx;
            y = miny;
            return true;
        }

        int x0, y0, g;
        if (!solve_any(a, b, c, x0, y0, g)) return false;
        if (g == 0) return false;

        a /= g; b /= g;
        int dx = b;
        int dy = a;

        int kx_min, kx_max, ky_min, ky_max;
        if (!k_range_x(x0, dx, minx, maxx, kx_min, kx_max)) return false;
        if (!k_range_y(y0, dy, miny, maxy, ky_min, ky_max)) return false;

        int kmin = max(kx_min, ky_min);
        int kmax = min(kx_max, ky_max);
        if (kmin > kmax) return false;

        int k = kmin;
        x = x0 + k * dx;
        y = y0 - k * dy;
        return true;
    }
};
