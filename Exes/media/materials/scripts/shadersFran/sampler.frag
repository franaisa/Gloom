uniform sampler2D Depth;

void main()
{
    gl_FragColor = texture2D(Depth, gl_TexCoord[0].xy);
}