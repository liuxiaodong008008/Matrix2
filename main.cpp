#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include <iostream>
#include <type_traits>
#include "include/matrix.h"
#include "include/iterator.h"
#include "include/builder.h"
using namespace Mx;
using namespace std;

int main() {
    Matrix<int> mat(3,3,0);
    mat->inputter()<<1,2,3,4,5,6,7,8,9;
    cout<<mat<<endl;

    auto mat2 = mat->row({0,1,0});
    cout<<mat2<<endl;

    auto mat3 = mat->col(0) % mat2->col_wise();
    cout<<mat3<<endl;

    cout<<(mat3->col_wise()+mat3->col(0)->copy())<<endl;
	_CrtDumpMemoryLeaks();
}