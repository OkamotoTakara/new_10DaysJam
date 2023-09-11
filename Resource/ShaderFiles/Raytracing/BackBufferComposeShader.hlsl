
//入力情報
RWTexture2D<float4> BackBuffer : register(u0);

//入力情報
RWTexture2D<float4> Raytracing : register(u1);

//出力先UAV  
RWTexture2D<float4> OutputImg : register(u2);

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    //背景色だったらレイトレの結果を描画
    bool isBackGroundColor = BackBuffer[DTid.xy].a <= 0.0f;
    if (isBackGroundColor)
    {
    
        OutputImg[DTid.xy] = Raytracing[DTid.xy];
        
    }
    else
    {
        float alpha = BackBuffer[DTid.xy].w;
        OutputImg[DTid.xy] = BackBuffer[DTid.xy] * alpha + Raytracing[DTid.xy] * (1.0f - alpha);
    
    }
    
}