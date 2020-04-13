#include <iostream>
#include <random>

using namespace std;

void test(discrete_distribution<int>&distro, default_random_engine& dre){
    for(int i=0;i<100;++i){
        cout<<distro(dre);
    }
    cout<<endl;
}

int main()
{
    default_random_engine dre;
    discrete_distribution<int> distro({0,1,1,1});
    test(distro, dre);
    distro.param({0,1,1,1,1});
    test(distro, dre);
    distro.param({0,3,3,3,1});
    test(distro, dre);
    distro.param({0,25,25,25,15,10});
    test(distro, dre);
    distro.param({0,20,20,20,20,15,5});
    test(distro, dre);
    distro.param({0,20,20,20,20,10,5,5});
    test(distro, dre);
    distro.param({0,16,16,16,16,16,10,5,5});
    test(distro, dre);
    distro.param({0,13,13,13,13,13,13,10,5,5});
    test(distro, dre);
    distro.param({0,11,11,11,11,11,11,11,10,5,5});
    test(distro, dre);
    distro.param({0,10,10,10,10,10,10,10,10,10,5,5});
    test(distro, dre);
    distro.param({0,9,9,9,9,9,9,9,9,9,10,5,5});
    test(distro, dre);
    distro.param({0,8,8,8,8,8,8,8,8,8,8,10,5,5});
    test(distro, dre);
    distro.param({0,7,7,7,7,7,7,7,7,7,7,7,10,5,5});
    test(distro, dre);
    distro.param({0,7,7,7,7,7,7,7,7,7,7,7,7,10,5,5});
}