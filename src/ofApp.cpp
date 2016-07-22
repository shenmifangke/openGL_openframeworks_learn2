#include "ofApp.h"
/*
http://bullteacher.com/category/zh_learnopengl_com


http://bullteacher.com/7-textures.html
http://learnopengl-cn.readthedocs.org/zh/latest/


http://www.learnopengl.com/#!Code-repository


*/

string  ofApp::loadFragmentShader( string sPath )
{
    ofFile F;
//    F.clear();
//    F.close();
    F.open(ofToDataPath(sPath), ofFile::ReadWrite, false);
    string ccc = F.readToBuffer().getText();
    return ccc;
}


GLuint EBO;//����������� ��������α���ʹ������
GLuint vbo;//���㻺�����
GLuint vao;//�����������
const char* vertex_shader;
const char* fragment_shader;
GLuint shader_programme;
//core-profileģʽ �����°汾��OpenGL������3.3Ϊ����
//����ʹ�õľ���glfw3
void ofApp::setup(){

GLint nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);//һ����ڵ���16
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//float points[] = {
//     ��һ��������
//    0.5f, 0.5f, 0.0f,   // ���Ͻ�
//    0.5f, -0.5f, 0.0f,  // ���½�
//    -0.5f, 0.5f, 0.0f,  // ���Ͻ�
//
//     �ڶ���������
//    0.5f, -0.5f, 0.0f,  // ���½�
//    -0.5f, -0.5f, 0.0f, // ���½�
//    -0.5f, 0.5f, 0.0f   // ���Ͻ�
//};
//���ǾͿ��Ի������������������һ�����Σ�OpenGL��Ҫ���ǻ��������Σ� �����������˷� �����������������������
//GLfloat points[] = {
//
//    0.5f, 0.5f, 0.0f,   // ���Ͻ�
//    0.5f, -0.5f, 0.0f,  // ���½�
//    -0.5f, -0.5f, 0.0f, // ���½�
//    -0.5f, 0.5f, 0.0f   // ���Ͻ�
//};
//�����������ڷ��͵�������ɫ�������ݸ����ˣ��б�Ҫ����������ɫ����ʹ���ܹ�����ɫֵ��Ϊһ�������������롣��Ҫע�����������layout��ʶ��������ɫ���Ե�location����Ϊ1��
//GLfloat points[] = {
//    // λ��          // ��ɫ
//    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // ����
//    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // ����
//    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // ��
// };

GLfloat points[] = {
    // Positions // Colors // Texture Coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left
};
GLuint indices[] = { // ��ʼ��0! �������� ����Ƕ�����Ǳر���

    0, 1, 3, // ��һ��������
    1, 2, 3  // �ڶ���������
};
//----------------------------------------------------vao��


glGenBuffers(1, &EBO);//��������α���ʹ������
//ELEMENT�������������EBO����IBO��
//3. �������ǵ��������鵽һ�����������У��ṩ��OpenGLʹ��
//һ��EBO��һ���붥�㻺�����һ���Ļ��壬��ר�Ŵ���������OpenGL������Щ��������������ơ��������������������Ľ��������
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);


//VAO�������Ϊһ��״̬��������¼VBO��״̬��VAO��OpenGL 3.0�Ժ��������¶��� ��һ�����������԰������VBO,  ����������ǰ��call list, ��������һ����VBO�������У����Ի���Ч�ʽ���VBO�Ļ����ϸ���һ����
//�����������Vertex Array ObjectҲ����VAO�������񶥵㻺�����һ���󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С�
//VAO��ȫ����Vertex Array Object�����ȣ�������Buffer-Object�����Բ������洢���ݣ���Σ�����ԡ����㡱���ԣ�Ҳ����˵����������Ļ��ơ�ϢϢ��ء���VAO��VAû���κι�ϵ��
vao = 0;
glGenVertexArrays (1, &vao);
glBindVertexArray (vao);



//VBO��VA��Vertex Array���������� VBO��������Ϣ�ŵ�GPU�� VBO��ʵ�����Կ��е��Դ棬Ϊ�������Ⱦ�ٶȣ����Խ�Ҫ���ƵĶ������ݻ������Դ��У������Ͳ���Ҫ��Ҫ���ƵĶ��������ظ���CPU���͵�GPU, �˷Ѵ�����Դ��
//���㻺�����vertex buffer objects, VBO��ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�
//GPU����Ⱦʱȥ������ȡ���ݣ������м��������GL-Context��GL-Context��������һ��ֻ��һ�����������һ����Ⱦ�����������ݲ�ͬ�Ļ��ƴ��룬GL-context�͸���������֮������л���
//��Ҳ��ΪʲôҪ����Ⱦ�����У���ÿ�ݻ��ƴ���֮�л���glBindbuffer��glEnableVertexAttribArray��glVertexAttribPointer����ô�Ż��ķ������ˣ�����Щ���ŵ���ʼ��ʱ����ɰɣ� VAO��¼�ôλ�������Ҫ������VBO������Ϣ���������浽VBO�ض�λ�ã����Ƶ�ʱ��ֱ�������λ��ȡ��Ϣ���ơ�
vbo = 0;
glGenBuffers (1, &vbo);//���������һ����һ�޶���ID ʹ��glGenBuffers��������һ������ID OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡�
glBindBuffer (GL_ARRAY_BUFFER, vbo);//����һ��������ʹ�õ��κλ��庯������GL_ARRAY_BUFFERĿ���ϣ������������õ�ǰ�󶨵Ļ��壨VBO��
glBufferData (GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);//glBufferData��һ���������û��������ݸ��Ƶ���ǰ�󶨻���ĺ��� �м�Ҳ����sizeof (points)




// 3. ���ö�������ָ��
//����Ҳ�ܷ��ڰ�֮ǰ
//����OpenGL��ν��Ͷ��������ˣ�ÿһ���������ԣ�

//��һ������ָ������Ҫ������һ���������ԡ���ס�������ڶ�����ɫ����ʹ��layout(location = 0)�����˶������ԡ���λ�ã�position����location������Ҫ�Ѷ������Ե�location����Ϊ0����Ϊ����ϣ�������ݴ��ݵ�������������У�����������������0��
//��һ������ָ���������ԵĴ�С������������vec3���ͣ�����3����ֵ���
//����������ָ�����ݵ����ͣ�������GL_FLOAT��GLSL��vec*���ɸ�������ɵģ���
//���ĸ��������������Ƿ�ϣ�����ݱ���׼���������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
//�������������������stride���������������������Ķ�������֮�����ж��١������¸�λ��������3��GLfloat�����λ�ã����ǰѲ�������Ϊ3 * sizeof(GLfloat)��Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
//���һ�������йŹֵ�GLvoid*��ǿ������ת���������ǵ�λ�������ڻ�������ʼλ�õ�ƫ����������λ������������Ŀ�ʼ������������0
// ��������
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);//����OpenGL��ν��Ͷ������ݣ�����ҲҪ������������
// ��ɫ����
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
glEnableVertexAttribArray(1);
// UV���� ��������ӵ�
glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid * )(6*sizeof(GLfloat)));
glEnableVertexAttribArray(2);



glBindVertexArray(0);//�⿪vao�� ͨ������µ��������ú������Ժ�Ҫ���OpenGL�����������ǲŲ�����ĳ��������������ǡ������EBO����ע����󻺳��൱��û�а󶨻��壬�����ڽ��VAO֮���󻺳壩
//----------------------------------------------------vao��





string a = loadFragmentShader("fragment.frag");
string b = loadFragmentShader("vertex_shader.vert");


fragment_shader = a.c_str();//������string��������һ�ζ������ �������δ֪ԭ�� ������ִ���ٶ����δ����
vertex_shader = b.c_str();


GLint success;//�Ƿ����
GLchar infoLog[512];//���������Ϣ������



//��̬����glsl��Դ�� ����һ����ɫ�������ٴ���������ID���������Ǵ������������ɫ��ΪGLuint��Ȼ����glCreateShader������ɫ��
GLuint vs = glCreateShader (GL_VERTEX_SHADER);//����ɫ���������ṩglCreateShader��Ϊ���Ĳ������������Ǵ��ݵĲ�����GL_VERTEX_SHADER�����ʹ�����һ��������ɫ��
glShaderSource (vs, 1, &vertex_shader, NULL);//�������ɫ��Դ�븽�ӵ���ɫ������Ȼ������� �ڶ�����ָ����Դ�����ж��ٸ��ַ���������ֻ��һ���������������Ƕ�����ɫ��������Դ��
glCompileShader (vs);//�ⲽ֮����ܽ��д�����
//vs������ɫ��������
glGetShaderiv(vs, GL_COMPILE_STATUS, &success);//����������
if(!success)
{
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}



GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);//ʹ��GL_FRAGMENT_SHADER��Ϊ��ɫ������
glShaderSource (fs, 1, &fragment_shader, NULL);
glCompileShader (fs);
//fsƬ����ɫ��������
glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(fs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRANMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
}


//��ɫ���������shader program object���Ƕ����ɫ��������ӵİ汾�����Ǳ������������Ϊһ����ɫ��������󣬵���Ⱦ���弤����ɫ������
//����ɫ������Ϊһ������͵��ڰ�ÿ����ɫ����������ӵ���һ����ɫ�������롣
shader_programme = glCreateProgram ();
glAttachShader (shader_programme, fs);
glAttachShader (shader_programme, vs);
glLinkProgram (shader_programme);

//��ɫ��������󸽼Ӵ�����
glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);

if(!success) {
    glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
    std::cout << "last shader program object error \n" << infoLog << std::endl;
}


//���ɾ����ɫ������
glDeleteShader(vs);
glDeleteShader(fs);
//OpenGL�����ṩ16������Ԫ����ʹ�ã�Ҳ����˵����Լ���GL_TEXTURE0��GL_TEXTRUE15�����Ƕ���˳����ģ���������Ҳ����ͨ��GL_TEXTURE0+8�ķ�ʽ���GL_TEXTURE8����������ڵ����ǲ��ò�ѭ�����������ʱ���ú����á�
//////////////////////////////���ɰ����� ����û��ʹ�ý̳��е�Simple OpenGL Image Library������OpenGLͼ��⣩����ʹ����of�Դ���freeimage��
ofImage image;//ʹ��of�Դ���ת��image.getPixels() ����ת��Ϊconst GLvoid*��ʽ
image.loadImage("1.png");//����dataĿ¼��
ofImage image2;
image2.loadImage("2.jpg");


glGenTextures(1, &texture1);
glBindTexture(GL_TEXTURE_2D, texture1);
// Set the texture wrapping/filtering options (on the currently bound texture object)
//������÷�ʽ��Texture Wrapping��
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//������ˣ�Texture Filtering��
//�м���Ŀ ������˿������÷Ŵ����С��ѡ������������������С��ʱ��ʹ�����ٽ����ˣ����Ŵ�ʱʹ�����Թ��ˡ����Ǳ���ͨ��glTexParameter*Ϊ�Ŵ����Сָ�����˷�ʽ��
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Load and generate the texture
//int width, height;
//unsigned char * image = SOIL_load_image("container.jpg", &width, &eight, 0, SOIL_LOAD_RGB);

//��һ������ָ������Ŀ�꣨������������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ�꣨target���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
//�ڶ�������Ϊ���Ǵ��㴴��������ָ��mipmap�㼶�������ϣ��Ϊÿ��mimap�㼶�����ֹ����õĻ�������������0��������
//��������������OpenGL������ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ��������ǰ�������ΪRGBֵ��
//���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ����Ǽ���ͼ���ʱ����ǰ���������������Ǿ���ʹ����Ӧ�����ˡ�
//����������Ӧ��һֱ����Ϊ0���������⣩��
//���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ�����char(byte)
//���һ����������ʵ��ͼ�����ݡ�
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels());//����ĳ���RGBA ��Ϊ�õ���͸��png
glGenerateMipmap(GL_TEXTURE_2D);
//SOIL_free_image_data(image);
glBindTexture(GL_TEXTURE_2D, 0);//�������������Ӧ��mipmap�󣬽����������ͷ�ͼ����ڴ����Ҫ��




glGenTextures(2, &texture2);
glBindTexture(GL_TEXTURE_2D, texture2);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image2.width, image2.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2.getPixels());//����ĳ���RGB ��Ϊ�õ���jpg
glGenerateMipmap(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 1);

}

//--------------------------------------------------------------
void ofApp::update(){

}
#include <GLFW/glfw3.h>//glfwGetTime������Ҫ����
//--------------------------------------------------------------
void ofApp::draw(){
glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//�������draw�� glClearColor������һ��״̬���ú���
glClear(GL_COLOR_BUFFER_BIT);//glClear��һ��״̬ʹ�ú���





  // wipe the drawing surface clear
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (shader_programme);//// ������ɫ�� ����glUseProgram���������´����ĳ��������Ϊ���Ĳ������������ܼ�������������


   // glBindTexture(GL_TEXTURE_2D, texture1);//���Ƶ�ʱ����Ҫ����һ�������

//����������
glActiveTexture(GL_TEXTURE0); //�ڰ�����֮ǰ���ȼ�������Ԫ �������ʱ����Ժ��������Ԫ
glBindTexture(GL_TEXTURE_2D, texture1);
glUniform1i(glGetUniformLocation(shader_programme, "ourTexture1"), 0);//��һ�����ʱ����Ժ������ֵ

glActiveTexture(GL_TEXTURE1); //��������Ԫ2
glBindTexture(GL_TEXTURE_2D, texture2);
glUniform1i(glGetUniformLocation(shader_programme, "ourTexture2"), 1);



  glBindVertexArray (vao);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//��GL_ELEMENT_ARRAY_BUFFER��������Ŀ��  ��ͬ������һ�ֻ��Ʒ�ʽ
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao);//��GL_ELEMENT_ARRAY_BUFFER��������Ŀ��  ��ͬ������һ�ֻ��Ʒ�ʽ
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) ��ģʽ

  //��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ��
  //�ڶ������������Ǵ�����ƶ���Ĵ�����������6��˵�������ܹ������6�����㡣
  //���������������������ͣ�������GL_UNSIGNED_INT��
  //���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬������ֻ�ǵ��㲻����ʹ��������������ʱ�򣩣���������ֻ������������д0
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//��glDrawElements���滻glDrawArray ��ָ�����Ǵ�����������Ⱦ

  // draw points 0-3 from the currently bound VAO with current in-use shader
  //��һ�����������Ǵ�����Ƶ�OpenGL����ͼ�ε�����
  //�ڶ����������������Ǵ�����Ƶ��Ǹ������������ʼλ�õ�����������������0
  //���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ�������Ⱦһ�������Σ���ֻ��3�����㣩��
  //glDrawArrays (GL_TRIANGLES, 0, 3);



  glBindVertexArray(0);//�⿪vao��
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
