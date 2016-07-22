#version 330 core 
layout (location = 0) in vec3 position; // 位置变量的属性position为 0 
layout (location = 1) in vec3 color; // 颜色变量的属性position为 1 
layout (location = 2) in vec2 texCoord;//新增的纹理
out vec3 ourColor; // 向fragment shader输出一个颜色 
 out vec2 TexCoord;//新增的纹理
void main() 
{ 
    gl_Position = vec4(position, 1.0f); 
    ourColor = color; // 把ourColor设置为我们从顶点数据那里得到的输入颜色
	TexCoord = texCoord;//新增的纹理
}