//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_UTILS_H
#define BOSSCRIPT_UTILS_H

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif //BOSSCRIPT_UTILS_H
