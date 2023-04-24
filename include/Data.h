#ifndef DATA_H
#define DATA_H

#include "AttributeGroup.h"
#include "IntegerWrapper.h"
#include "Permutation.h"
#include "Destroy.h"
#include "Define.h"
#include "Vertex.h"
#include "Array.h"
#include "Graph.h"
#include "Tuple.h"
#include "Tree.h"
#include "Node.h"
#include "Edge.h"
#include "Pre.h"
#include "IO.h"

#define BUILD(ele) \
    ele->generate(0); \
    ele->out(); \
    Destroy::get()->start_service(); \
    ele->destroy(); \
    Destroy::get()->stop_service();

#endif