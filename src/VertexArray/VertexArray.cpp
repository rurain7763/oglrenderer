#include "VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"
#include "../Logger/Logger.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &_vao);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_vao);
}

void VertexArray::Init(const VertexBuffer& vbo, const VertexBufferLayoutGroup& layoutGroup) {
    glBindVertexArray(_vao);

    vbo.Bind();
    int i = 0;
    size_t offset = 0;
    for(auto& layout : layoutGroup.GetLayouts()) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, layout.count, layout.type, layout.normalizedFlag, layoutGroup.GetStride(), (const void*)offset);
        i++;
        offset += layout.typeSize * layout.count;
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(_vao);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}