#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    std::ostringstream oss;
    oss<<"controller-II.exe ";
    for(int i=1;i<argc;i++){
        oss<<argv[i]<<" ";
    }
    oss<<std::endl;
    //std::cout<<oss.str()<<std::endl;
    std::cout<<system(oss.str().c_str())<<std::endl;
    return 0;
}