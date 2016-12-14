#include <Observer.h>

namespace obs {

namespace detail {

UniversalPtr createHeartBeat() {
  return createEmptyPtr([] (void*) {});
}

}

}
