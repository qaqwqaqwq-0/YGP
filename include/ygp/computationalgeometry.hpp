#ifndef _YGP_COMPUTATIONALGEOMETRY_HPP_
#define _YGP_COMPUTATIONALGEOMETRY_HPP_
#include"configure.hpp"
#include<cmath>
#include<ostream>
#ifndef YGP_MIN_PREC
#define YGP_MIN_PREC 0.000001f
#endif
BEGIN_NAMESPACE_YGP
namespace cg
{
    struct line;
    struct vector;
    struct polygon;
    struct segment;
    struct point
    {
        double x;
        double y;
        point():x{0.0f},y{0.0f}{}
        point(const point& p):x(p.x),y(p.y){}
        point(double X,double Y):x(X),y(Y){}
        bool operator==(const point& p)const noexcept
        {
            return fabs(x-p.x)<YGP_MIN_PREC&&fabs(y-p.y)<YGP_MIN_PREC;
        }
        point& operator=(const point& p)noexcept
        {
            x=p.x,y=p.y;return *this;
        }
        double distance(const point& p)const noexcept
        {
            double dx=x-p.x,dy=y-p.y;
            return sqrt(dx*dx+dy*dy);
        }
        bool onsameline(const point& p2,const point& p3)
        {
            double dx12=p2.x-x,dx23=p3.x-p2.x,
                dy12=p2.y-y,dy23=p3.y-p2.y;
            return fabs(dy12*dx23-dx12*dy23)<YGP_MIN_PREC;
        }
        point operator+(const vector&);
        bool on(const line&)const noexcept;
        bool on(const segment&)const noexcept;
        bool in(const polygon&)const;
    };
    struct vector
    {
        double x;
        double y;
        vector():x{0.0f},y{0.0f}{}
        vector(const vector& v):x(v.x),y(v.y){}
        vector(double X,double Y):x(X),y(Y){}
        vector(const point& p1,const point& p2):x(p2.x-p1.x),y(p2.y-p1.y){}
        double length()const noexcept
        {
            return sqrt(x*x+y*y);
        }
        double k()const noexcept
        {
            return y/x;
        }
        bool operator==(const vector& v)const noexcept
        {
            return fabs(x-v.x)<YGP_MIN_PREC&&fabs(y-v.y)<YGP_MIN_PREC;
        }
        vector& operator=(const vector& v)noexcept
        {
            x=v.x,y=v.y;return *this;
        }
        vector operator*(double f)const noexcept
        {
            return vector(x*f,y*f);
        }
        vector& operator*=(double f)noexcept
        {
            x*=f,y*=f;return *this;
        }
        vector operator/(double f)const noexcept
        {
            return vector(x/f,y/f);
        }
        vector& operator/=(double f)noexcept
        {
            x/=f,y/=f;return *this;
        }
        vector& operator+=(const vector& v)noexcept
        {
            x+=v.x,y+=v.y;return *this;
        }
        vector operator+(const vector& v)const noexcept
        {
            return vector(x+v.x,y+v.y);
        }
        vector& operator-=(const vector& v)noexcept
        {
            x-=v.x,y-=v.y;return *this;
        }
        vector operator-(const vector& v)const noexcept
        {
            return vector(x-v.x,y-v.y);
        }
        double operator*(const vector& v)const noexcept//cross product
        {
            return x*v.y-y*v.x;
        }
        double operator^(const vector& v)const noexcept//dot product
        {
            return x*v.x+y*v.y;
        }
    };
    point point::operator+(const vector& v)
    {
        return point(x+v.x,y+v.y);
    }
    struct line
    {
        double A,B,C;//Ax+By+C=0
        line():A{0.0f},B{0.0f},C{0.0f}{}
        line(double k,double b):B{1.0f},A{-k},C{-b}{}//assumes that k!=nan
            //k=-A/B,b=-C/B
        line(double a,double b,double c):A(a),B(b),C(c){}
        line(const line& l):A(l.A),B(l.B),C(l.C){}
        line(const point& p1,const point& p2):A{p2.y-p1.y},B{p1.x-p2.x},
            C{p1.x*(p1.y-p2.y)+p1.y*(p2.x-p1.x)}{}
        line(const point& p,const vector& v):A{v.y},B{-v.x},C{-v.y*p.x+v.x*p.y}{}
        double k()const noexcept
        {
            return -A/B;
        }
        double b()const noexcept
        {
            return -C/B;
        }
        double y(double x)const noexcept
        {
            return k()*x+b();
        }
    };
    bool point::on(const line& l)const noexcept
    {
        return fabs(l.A*x+l.B*x+l.C)<YGP_MIN_PREC;
    }
    struct segment
    {
        point p1;
        point p2;
        segment(){}
        segment(const point& P1,const point& P2):p1{P1},p2{P2}{}
        segment(const point& p,const vector& v):p1{p},p2{p1+v}{}
        operator vector()const noexcept
        {
            return vector(p1,p2);
        }
        operator line()const noexcept
        {
            return line(p1,p2);
        }
        double k()const noexcept
        {
            return (p2.y-p1.y)/(p2.x-p1.x);
        }
        double b()const noexcept
        {
            return (p1.y*p2.x-p2.y*p1.x)/(p2.x-p1.x);
        }
    };
    struct polygon
    {
        int n;
        point* p;
        polygon():p{nullptr},n{0}{}
        polygon(int _n):n{_n},p{new point[_n]}{}
        bool empty()const noexcept
        {
            return !n;
        }
        ~polygon(){if(p)delete []p;}
    };
    // bool point::in(const polygon& poly)const
    // {
    //     if(poly.empty())
    //         throw std::invalid_argument("ygp::cg::point::in: "
    //             "The parameter is an empty polygon.");
    //     for(int i=0;i<poly.n-1;++i)
    //     {

    //     }
    // }
    std::ostream& operator<<(std::ostream& os,const point& p)
    {
        return os<<"point("<<p.x<<','<<p.y<<")";
    }
    std::ostream& operator<<(std::ostream& os,const vector& v)
    {
        return os<<"vector("<<v.x<<','<<v.y<<")";
    }
    std::ostream& operator<<(std::ostream& os,const line& l)
    {
        os<<"line("<<l.A<<'x';
        if(l.B>0)os<<'+';
        os<<l.B<<'y';
        if(l.C>0)os<<'+';
        if(l.C)os<<l.C;
        os<<"=0)";
        return os;
    }
    std::ostream& operator<<(std::ostream& os,const segment& s)
    {
        return os<<"segment(("<<s.p1.x<<','<<s.p1.y<<")->("<<s.p2.x<<','<<s.p2.y<<"))";
    }
}
namespace cg2d=cg;//2-dimension
namespace cg3d
{
    struct point
    {
        double x;
        double y;
        double z;
        point():x{0.0f},y{0.0f},z{0.0f}{}
        point(double X,double Y,double Z):x{X},y{Y},z{Z}{}
    };
};
END_NAMESPACE_YGP
#endif