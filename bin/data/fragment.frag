#version 330 core 
in vec3 ourColor;
in vec2 TexCoord;//新增的纹理
out vec4 color; 
uniform sampler2D ourTexture1;//新增的纹理采样器
uniform sampler2D ourTexture2;//第二个纹理
void main() 
{ 
vec2 texCoord = vec2(TexCoord.x,1 - TexCoord.y);//上下颠倒
    //color = vec4(ourColor, 1.0f);
	color = texture(ourTexture1, TexCoord);//我们使用GLSL的内建texture函数来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是相应的纹理坐标
//简单混合
color = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);
//图片混合
color = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.5);
}
