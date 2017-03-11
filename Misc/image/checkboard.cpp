//xy像素分辨率，*_res密度，引用将会返回最终选择的合适密度
//切记删除返回的浮点数组
float* generate_checkerboard(int x,int y,int& x_res,int& y_res)
{
	if(x<=0||y<=0)
	{
		printf("fuck off!\n");
		return nullptr;
	}
	if(x_res<=0) x_res = 1;
	if(y_res<=0) y_res = 1;


	//correct checker resolution
	while(x%x_res!=0) x_res--;
	while(y%y_res!=0) y_res--;
	int intervalx = x/x_res;
	int intervaly = y/y_res;
	float val = 0;
	float* mem = new float[x*y];
	float* mem0 = new float[intervalx];
	float* mem1 = new float[intervalx];
	float* mem00 = new float[x];
	float* mem11 = new float[x];
    int cpsize = intervalx*sizeof(float);


    //make a element line
	std::memset(mem0,0,cpsize);
	for(int i=0;i<intervalx;++i)
		mem1[i]=1.f;

    //make line
	int temp = x_res/2;
    int dbix = intervalx*2;
	for(int i=0;i<temp;++i)
	{
		std::memcpy(&mem00[i*dbix+intervalx],mem1,cpsize);
		std::memcpy(&mem11[i*dbix+intervalx],mem0,cpsize);

	}
	temp = x_res - temp;
	for(int i=0;i<temp;++i)
	{
		std::memcpy(&mem00[i*dbix],mem0,cpsize);
		std::memcpy(&mem11[i*dbix],mem1,cpsize);
	}

    //make element block
	for(int i=0;i<intervaly;++i)
	{
		std::memcpy(&mem[i*x],mem00,x*sizeof(float));

	}
	if(y_res!=1)
	{
        int intervalyx = intervaly*x;
	for(int i=0;i<intervaly;++i)
		std::memcpy(&mem[i*x+intervalyx],mem11,x*sizeof(float));
	}

    //finish
	int nyr = y_res - 2;
	temp = nyr/2;
    int xintervaly2 = x*intervaly*2;
    int xintervaly3 = x*intervaly*3;
    int ix = intervaly*x;
    int ixs = ix*sizeof(float);
	for(int i=0;i<temp;++i)
	{
		std::memcpy(&mem[i*xintervaly2+xintervaly3],&mem[ix],ixs);
	}
	temp = nyr - temp;
	for(int i=0;i<temp;++i)
	{
        std::memcpy(&mem[i*xintervaly2+xintervaly2],mem,ixs);
	}


	delete[] mem0;
	delete[] mem1;
	delete[] mem00;
	delete[] mem11;

	return mem;
}