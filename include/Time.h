typedef std::chrono::steady_clock hr_clock;
typedef std::chrono::duration<float> time_in_seconds;

class Time {
private:
    Time();
    inline static hr_clock::time_point _start_point;
    inline static hr_clock::time_point _last_time;
    inline static time_in_seconds _delta_time;
public:
    inline static void init(){
        _start_point = hr_clock::now();
        _last_time = _start_point;
        _delta_time = time_in_seconds(0);
    }

    inline static void tick(){
        auto now = hr_clock::now();
        _delta_time = std::chrono::duration_cast<time_in_seconds>(now - _last_time);
        _last_time = now;
    }

    inline static time_in_seconds elapsed_time(){
        return std::chrono::duration_cast<time_in_seconds>(hr_clock::now() - _start_point);
    }

    inline static time_in_seconds delta_time(){
        return _delta_time;
    }
};