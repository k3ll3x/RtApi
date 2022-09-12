namespace Chin {
    void sempai(void* x);

    class Chong {
    public:
        Chong(){}
        void i();
    private:
        int x;
    };
}

namespace Jin {
    namespace Jan {
        class JinJan {
        public:
            JinJan();
        private:
            int x;
        };
    }
}

template<class T, class J = int>
class Type {
public:
    J x;
    Type();
    Type(T t){}
    Type(J j);
    Type(T j, J j);
    T* ptr(T t);
private:
    T y;
};

T* Type::ptr(T t){
    return &t;
}

class ExampleClass {
public:
    ExampleClass(int x){
        _x = x;
        y = new int();
    }

    ~ExampleClass(){
        delete y;
    }

    int* ptrY(){
        return y;
    }

    int* ptrX(){
        return &_x;
    }
    
    Type<int,int> sempai(const double& a, std::vector<std::string>& b, int c);

    Type<int> sempoi(const double& a, std::vector<std::string>& b, Type<int> c, int * x)
    {
        return c;
    }

    void yaqui(int z){
    }

    int yaqui(int *z){
        return *z;
    }

    void masai(double x);

    float shalomo(double x){
        return (float)(x + *y)/x;
    }
private:
    int _x;
    int *y;
};

auto lfun(ExampleClass e){
    return e.getX();
}

void other(int a, int b);