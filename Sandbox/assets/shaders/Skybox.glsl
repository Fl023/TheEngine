 #type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 texCoords;

uniform mat4 u_ViewProjection;

void main()
{
    vec4 pos = u_ViewProjection * vec4(a_Position, 1.0f);
    // Having z equal w will always result in a depth of 1.0f
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    // We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
    texCoords = vec3(a_Position.x, a_Position.y, -a_Position.z);
}   



#type fragment
#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube u_Skybox;

void main()
{    
    FragColor = texture(u_Skybox, texCoords);
}