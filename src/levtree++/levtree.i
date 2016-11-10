%module levtree
%{
#include "levtree++/Levtree.hpp"
%}

%include "std_string.i"
%include "std_vector.i"

// Instantiate templates used by example
namespace std
{
    %template(ResultVector) vector<levtree::LevtreeResult>;
    %template(StringVector) vector<string>;
}

%include "levtree/common.h"
%include "levtree++/Levtree.hpp"
