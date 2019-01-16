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
    Matrix<double> mat(3,3,0);
    mat->inputter()<<1,2,3,4,5,6,7,8,9;
    cout<<mat<<endl;

    auto mat2 = mat->col(0,2);

    RowwiseBuilder<double> mb;
    for(auto r:mat2->row_items()) {
        mb.add(r);
    }
    cout<<mb.build()<<endl;

	_CrtDumpMemoryLeaks();
}