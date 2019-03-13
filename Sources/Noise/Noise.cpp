#include "Noise.hpp"

#include <random>

namespace acid
{
// Hashing
static const int32_t X_PRIME = 1619;
static const int32_t Y_PRIME = 31337;
static const int32_t Z_PRIME = 6971;
static const int32_t W_PRIME = 1013;

static const float GRAD_X[] = {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const float GRAD_Y[] = {1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f};
static const float GRAD_Z[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f};

static const float GRAD_4D[] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 1.0f, 1.0f, 0.0f,  -1.0f, 1.0f,  -1.0f, 0.0f,  -1.0f, -1.0f, 1.0f, 0.0f,  -1.0f, -1.0f, -1.0f,
								1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,  -1.0f, 1.0f, 1.0f, 0.0f,  -1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 1.0f, -1.0f, 0.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 1.0f, -1.0f, 0.0f,  -1.0f, -1.0f,
								1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  -1.0f, 1.0f, -1.0f, 0.0f,  1.0f, 1.0f, -1.0f, 0.0f,  -1.0f, -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,  -1.0f, -1.0f, -1.0f, 0.0f,  1.0f, -1.0f, -1.0f, 0.0f,  -1.0f,
								1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 1.0f,  1.0f, 0.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, -1.0f, -1.0f, 0.0f};

static const float VAL_LUT[] = {
	0.3490196078f,   0.4352941176f,   -0.4509803922f,  0.6392156863f,	0.5843137255f,  -0.1215686275f, 0.7176470588f,  -0.1058823529f,  0.3960784314f,   0.0431372549f,  -0.03529411765f,
	0.3176470588f,   0.7254901961f,   0.137254902f,	0.8588235294f,	-0.8196078431f, -0.7960784314f, -0.3333333333f, -0.6705882353f,  -0.3882352941f,  0.262745098f,   0.3254901961f,
	-0.6470588235f,  -0.9215686275f,  -0.5294117647f,  0.5294117647f,	-0.4666666667f, 0.8117647059f,  0.3803921569f,  0.662745098f,	0.03529411765f,  -0.6156862745f, -0.01960784314f,
	-0.3568627451f,  -0.09019607843f, 0.7490196078f,   0.8352941176f,	-0.4039215686f, -0.7490196078f, 0.9529411765f,  -0.0431372549f,  -0.9294117647f,  -0.6549019608f, 0.9215686275f,
	-0.06666666667f, -0.4431372549f,  0.4117647059f,   -0.4196078431f,   -0.7176470588f, -0.8117647059f, -0.2549019608f, 0.4901960784f,   0.9137254902f,   0.7882352941f,  -1.0f,
	-0.4745098039f,  0.7960784314f,   0.8509803922f,   -0.6784313725f,   0.4588235294f,  1.0f,			 -0.1843137255f, 0.4509803922f,   0.1450980392f,   -0.231372549f,  -0.968627451f,
	-0.8588235294f,  0.4274509804f,   0.003921568627f, -0.003921568627f, 0.2156862745f,  0.5058823529f,  0.7647058824f,  0.2078431373f,   -0.5921568627f,  0.5764705882f,  -0.1921568627f,
	-0.937254902f,   0.08235294118f,  -0.08235294118f, 0.9058823529f,	0.8274509804f,  0.02745098039f, -0.168627451f,  -0.7803921569f,  0.1137254902f,   -0.9450980392f, 0.2f,
	0.01960784314f,  0.5607843137f,   0.2705882353f,   0.4431372549f,	-0.9607843137f, 0.6156862745f,  0.9294117647f,  -0.07450980392f, 0.3098039216f,   0.9921568627f,  -0.9137254902f,
	-0.2941176471f,  -0.3411764706f,  -0.6235294118f,  -0.7647058824f,   -0.8901960784f, 0.05882352941f, 0.2392156863f,  0.7333333333f,   0.6549019608f,   0.2470588235f,  0.231372549f,
	-0.3960784314f,  -0.05098039216f, -0.2235294118f,  -0.3725490196f,   0.6235294118f,  0.7019607843f,  -0.8274509804f, 0.4196078431f,   0.07450980392f,  0.8666666667f,  -0.537254902f,
	-0.5058823529f,  -0.8039215686f,  0.09019607843f,  -0.4823529412f,   0.6705882353f,  -0.7882352941f, 0.09803921569f, -0.6078431373f,  0.8039215686f,   -0.6f,		   -0.3254901961f,
	-0.4117647059f,  -0.01176470588f, 0.4823529412f,   0.168627451f,	 0.8745098039f,  -0.3647058824f, -0.1607843137f, 0.568627451f,	-0.9921568627f,  0.9450980392f,  0.5137254902f,
	0.01176470588f,  -0.1450980392f,  -0.5529411765f,  -0.5764705882f,   -0.1137254902f, 0.5215686275f,  0.1607843137f,  0.3725490196f,   -0.2f,		   -0.7254901961f, 0.631372549f,
	0.7098039216f,   -0.568627451f,   0.1294117647f,   -0.3098039216f,   0.7411764706f,  -0.8509803922f, 0.2549019608f,  -0.6392156863f,  -0.5607843137f,  -0.3176470588f, 0.937254902f,
	0.9843137255f,   0.5921568627f,   0.6941176471f,   0.2862745098f,	-0.5215686275f, 0.1764705882f,  0.537254902f,   -0.4901960784f,  -0.4588235294f,  -0.2078431373f, -0.2156862745f,
	0.7725490196f,   0.3647058824f,   -0.2392156863f,  0.2784313725f,	-0.8823529412f, 0.8980392157f,  0.1215686275f,  0.1058823529f,   -0.8745098039f,  -0.9843137255f, -0.7019607843f,
	0.9607843137f,   0.2941176471f,   0.3411764706f,   0.1529411765f,	0.06666666667f, -0.9764705882f, 0.3019607843f,  0.6470588235f,   -0.5843137255f,  0.05098039216f, -0.5137254902f,
	-0.137254902f,   0.3882352941f,   -0.262745098f,   -0.3019607843f,   -0.1764705882f, -0.7568627451f, 0.1843137255f,  -0.5450980392f,  -0.4980392157f,  -0.2784313725f, -0.9529411765f,
	-0.09803921569f, 0.8901960784f,   -0.2862745098f,  -0.3803921569f,   0.5529411765f,  0.7803921569f,  -0.8352941176f, 0.6862745098f,   0.7568627451f,   0.4980392157f,  -0.6862745098f,
	-0.8980392157f,  -0.7725490196f,  -0.7098039216f,  -0.2470588235f,   -0.9058823529f, 0.9764705882f,  0.1921568627f,  0.8431372549f,   -0.05882352941f, 0.3568627451f,  0.6078431373f,
	0.5450980392f,   0.4039215686f,   -0.7333333333f,  -0.4274509804f,   0.6f,			 0.6784313725f,  -0.631372549f,  -0.02745098039f, -0.1294117647f,  0.3333333333f,  -0.8431372549f,
	0.2235294118f,   -0.3490196078f,  -0.6941176471f,  0.8823529412f,	0.4745098039f,  0.4666666667f,  -0.7411764706f, -0.2705882353f,  0.968627451f,	0.8196078431f,  -0.662745098f,
	-0.4352941176f,  -0.8666666667f,  -0.1529411765f,
};

static const float F3 = 1.0f / 3.0f;
static const float G3 = 1.0f / 6.0f;

static const float F2 = 0.5f * (std::sqrt(3.0f) - 1.0f);
static const float G2 = (3.0f - std::sqrt(3.0f)) / 6.0f;

static const float F4 = (std::sqrt(5.0f) - 1.0f) / 4.0f;
static const float G4 = (5.0f - std::sqrt(5.0f)) / 20.0f;

static const float CUBIC_2D_BOUNDING = 1.0f / (1.5f * 1.5f);
static const float CUBIC_3D_BOUNDING = 1.0f / (1.5f * 1.5f * 1.5f);

static const int32_t FN_CELLULAR_INDEX_MAX = 3;

static const float CELL_2D_X[] = {
	-0.6440658039f, -0.08028078721f, 0.9983546168f,  0.9869492062f,  0.9284746418f,   0.6051097552f,  -0.794167404f,  -0.3488667991f, -0.943136526f,   -0.9968171318f, 0.8740961579f,  0.1421139764f,   0.4282553608f,  -0.9986665833f, 0.9996760121f,  -0.06248383632f,
	0.7120139305f,  0.8917660409f,   0.1094842955f,  -0.8730880804f, 0.2594811489f,   -0.6690063346f, -0.9996834972f, -0.8803608671f, -0.8166554937f,  0.8955599676f,  -0.9398321388f, 0.07615451399f,  -0.7147270565f, 0.8707354457f,  -0.9580008579f, 0.4905965632f,
	0.786775944f,   0.1079711577f,   0.2686638979f,  0.6113487322f,  -0.530770584f,   -0.7837268286f, -0.8558691039f, -0.5726093896f, -0.9830740914f,  0.7087766359f,  0.6807027153f,  -0.08864708788f, 0.6704485923f,  -0.1350735482f, -0.9381333003f, 0.9756655376f,
	0.4231433671f,  -0.4959787385f,  0.1005554325f,  -0.7645857281f, -0.5859053796f,  -0.9751154306f, -0.6972258572f, 0.7907012002f,  -0.9109899213f,  -0.9584307894f, -0.8269529333f, 0.2608264719f,   -0.7773760119f, 0.7606456974f,  -0.8961083758f, -0.9838134719f,
	0.7338893576f,  0.2161226729f,   0.673509891f,   -0.5512056873f, 0.6899744332f,   0.868004831f,   0.5897430311f,  -0.8950444221f, -0.3595752773f,  0.8209486981f,  -0.2912360132f, -0.9965011374f,  0.9766994634f,  0.738790822f,   -0.4730947722f, 0.8946479441f,
	-0.6943628971f, -0.6620468182f,  -0.0887255502f, -0.7512250855f, -0.5322986898f,  0.5226295385f,  0.2296318375f,  0.7915307344f,  -0.2756485999f,  -0.6900234522f, 0.07090588086f, 0.5981278485f,   0.3033429312f,  -0.7253142797f, -0.9855874307f, -0.1761843396f,
	-0.6438468325f, -0.9956136595f,  0.8541580762f,  -0.9999807666f, -0.02152416253f, -0.8705983095f, -0.1197138014f, -0.992107781f,  -0.9091181546f,  0.788610536f,   -0.994636402f,  0.4211256853f,   0.3110430857f,  -0.4031127839f, 0.7610684239f,  0.7685674467f,
	0.152271555f,   -0.9364648723f,  0.1681333739f,  -0.3567427907f, -0.418445483f,   -0.98774778f,   0.8705250765f,  -0.8911701067f, -0.7315350966f,  0.6030885658f,  -0.4149130821f, 0.7585339481f,   0.6963196535f,  0.8332685012f,  -0.8086815232f, 0.7518116724f,
	-0.3490535894f, 0.6972110903f,   -0.8795676928f, -0.6442331882f, 0.6610236811f,   -0.9853565782f, -0.590338458f,  0.09843602117f, 0.5646534882f,   -0.6023259233f, -0.3539248861f, 0.5132728656f,   0.9380385118f,  -0.7599270056f, -0.7425936564f, -0.6679610562f,
	-0.3018497816f, 0.814478266f,	0.03777430269f, -0.7514235086f, 0.9662556939f,   -0.4720194901f, -0.435054126f,  0.7091901235f,  0.929379209f,	0.9997434357f,  0.8306320299f,  -0.9434019629f,  -0.133133759f,  0.5048413216f,  0.3711995273f,  0.98552091f,
	0.7401857005f,  -0.9999981398f,  -0.2144033253f, 0.4808624681f,  -0.413835885f,   0.644229305f,   0.9626648696f,  0.1833665934f,  0.5794129f,	  0.01404446873f, 0.4388494993f,  0.5213612322f,   -0.5281609948f, -0.9745306846f, -0.9904373013f, 0.9100232252f,
	-0.9914057719f, 0.7892627765f,   0.3364421659f,  -0.9416099764f, 0.7802732656f,   0.886302871f,   0.6524471291f,  0.5762186726f,  -0.08987644664f, -0.2177026782f, -0.9720345052f, -0.05722538858f, 0.8105983127f,  0.3410261032f,  0.6452309645f,  -0.7810612152f,
	0.9989395718f,  -0.808247815f,   0.6370177929f,  0.5844658772f,  0.2054070861f,   0.055960522f,   -0.995827561f,  0.893409165f,   -0.931516824f,   0.328969469f,   -0.3193837488f, 0.7314755657f,   -0.7913517714f, -0.2204109786f, 0.9955900414f,  -0.7112353139f,
	-0.7935008741f, -0.9961918204f,  -0.9714163995f, -0.9566188669f, 0.2748495632f,   -0.4681743221f, -0.9614449642f, 0.585194072f,   0.4532946061f,   -0.9916113176f, 0.942479587f,   -0.9813704753f,  -0.6538429571f, 0.2923335053f,  -0.2246660704f, -0.1800781949f,
	-0.9581216256f, 0.552215082f,	-0.9296791922f, 0.643183699f,   0.9997325981f,   -0.4606920354f, -0.2148721265f, 0.3482070809f,  0.3075517813f,   0.6274756393f,  0.8910881765f,  -0.6397771309f,  -0.4479080125f, -0.5247665011f, -0.8386507094f, 0.3901291416f,
	0.1458336921f,  0.01624613149f,  -0.8273199879f, 0.5611100679f,  -0.8380219841f,  -0.9856122234f, -0.861398618f,  0.6398413916f,  0.2694510795f,   0.4327334514f,  -0.9960265354f, -0.939570655f,   -0.8846996446f, 0.7642113189f,  -0.7002080528f, 0.664508256f,
};
static const float CELL_2D_Y[] = {
	0.7649700911f,  0.9967722885f,	0.05734160033f, -0.1610318741f,   0.371395799f,	-0.7961420628f, 0.6076990492f,  -0.9371723195f, 0.3324056156f,  0.07972205329f,  -0.4857529277f, -0.9898503007f,  0.9036577593f,  0.05162417479f, -0.02545330525f, -0.998045976f,
	-0.7021653386f, -0.4524967717f,   -0.9939885256f, -0.4875625128f,   -0.9657481729f,  -0.7432567015f, 0.02515761212f, 0.4743044842f,  0.5771254669f,  0.4449408324f,   0.3416365773f,  0.9970960285f,   0.6994034849f,  0.4917517499f,  0.286765333f,	0.8713868327f,
	0.6172387009f,  0.9941540269f,	0.9632339851f,  -0.7913613129f,   0.847515538f,	0.6211056739f,  0.5171924952f,  -0.8198283277f, -0.1832084353f, 0.7054329737f,   0.7325597678f,  0.9960630973f,   0.7419559859f,  0.9908355749f,  -0.346274329f,   0.2192641299f,
	-0.9060627411f, -0.8683346653f,   0.9949314574f,  -0.6445220433f,   -0.8103794704f,  -0.2216977607f, 0.7168515217f,  0.612202264f,   -0.412428616f,  0.285325116f,	0.56227115f,	-0.9653857009f,  -0.6290361962f, 0.6491672535f,  0.443835306f,	-0.1791955706f,
	-0.6792690269f, -0.9763662173f,   0.7391782104f,  0.8343693968f,	0.7238337389f,   0.4965557504f,  0.8075909592f,  -0.4459769977f, -0.9331160806f, -0.5710019572f,  0.9566512346f,  -0.08357920318f, 0.2146116448f,  -0.6739348049f, 0.8810115417f,   0.4467718167f,
	-0.7196250184f, -0.749462481f,	0.9960561112f,  0.6600461127f,	-0.8465566164f,  -0.8525598897f, -0.9732775654f, 0.6111293616f,  -0.9612584717f, -0.7237870097f,  -0.9974830104f, -0.8014006968f,  0.9528814544f,  -0.6884178931f, -0.1691668301f,  0.9843571905f,
	0.7651544003f,  -0.09355982605f,  -0.5200134429f, -0.006202125807f, -0.9997683284f,  0.4919944954f,  -0.9928084436f, -0.1253880012f, -0.4165383308f, -0.6148930171f,  -0.1034332049f, -0.9070022917f,  -0.9503958117f, 0.9151503065f,  -0.6486716073f,  0.6397687707f,
	-0.9883386937f, 0.3507613761f,	0.9857642561f,  -0.9342026446f,   -0.9082419159f,  0.1560587169f,  0.4921240607f,  -0.453669308f,  0.6818037859f,  0.7976742329f,   0.9098610522f,  0.651633524f,	0.7177318024f,  -0.5528685241f, 0.5882467118f,   0.6593778956f,
	0.9371027648f,  -0.7168658839f,   -0.4757737632f, 0.7648291307f,	0.7503650398f,   0.1705063456f,  -0.8071558121f, -0.9951433815f, -0.8253280792f, -0.7982502628f,  0.9352738503f,  0.8582254747f,   -0.3465310238f, 0.65000842f,	-0.6697422351f,  0.7441962291f,
	-0.9533555f,	0.5801940659f,	-0.9992862963f, -0.659820211f,	0.2575848092f,   0.881588113f,   -0.9004043022f, -0.7050172826f, 0.369126382f,   -0.02265088836f, 0.5568217228f,  -0.3316515286f,  0.991098079f,   -0.863212164f,  -0.9285531277f,  0.1695539323f,
	-0.672402505f,  -0.001928841934f, 0.9767452145f,  -0.8767960349f,   0.9103515037f,   -0.7648324016f, 0.2706960452f,  -0.9830446035f, 0.8150341657f,  -0.9999013716f,  -0.8985605806f, 0.8533360801f,   0.8491442537f,  -0.2242541966f, -0.1379635899f,  -0.4145572694f,
	0.1308227633f,  0.6140555916f,	0.9417041303f,  -0.336705587f,	-0.6254387508f,  0.4631060578f,  -0.7578342456f, -0.8172955655f, -0.9959529228f, -0.9760151351f,  0.2348380732f,  -0.9983612848f,  0.5856025746f,  -0.9400538266f, -0.7639875669f,  0.6244544645f,
	0.04604054566f, 0.5888424828f,	0.7708490978f,  -0.8114182882f,   0.9786766212f,   -0.9984329822f, 0.09125496582f, -0.4492438803f, -0.3636982357f, 0.9443405575f,   -0.9476254645f, -0.6818676535f,  -0.6113610831f, 0.9754070948f,  -0.0938108173f,  -0.7029540015f,
	-0.6085691109f, -0.08718862881f,  -0.237381926f,  0.2913423132f,	0.9614872426f,   0.8836361266f,  -0.2749974196f, -0.8108932717f, -0.8913607575f, 0.129255541f,	-0.3342637104f, -0.1921249337f,  -0.7566302845f, -0.9563164339f, -0.9744358146f,  0.9836522982f,
	-0.2863615732f, 0.8337016872f,	0.3683701937f,  0.7657119102f,	-0.02312427772f, 0.8875600535f,  0.976642191f,   0.9374176384f,  0.9515313457f,  -0.7786361937f,  -0.4538302125f, -0.7685604874f,  -0.8940796454f, -0.8512462154f, 0.5446696133f,   0.9207601495f,
	-0.9893091197f, -0.9998680229f,   0.5617309299f,  -0.8277411985f,   0.545636467f,	0.1690223212f,  -0.5079295433f, 0.7685069899f,  -0.9630140787f, 0.9015219132f,   0.08905695279f, -0.3423550559f,  -0.4661614943f, -0.6449659371f, 0.7139388509f,   0.7472809229f,
};
static const float CELL_3D_X[] = {
	0.3752498686f,   0.687188096f,	0.2248135212f,  0.6692006647f,  -0.4376476931f, 0.6139972552f,   0.9494563929f,   0.8065108882f,  -0.2218812853f,  0.8484661167f,	0.5551817596f,   0.2133903499f,  0.5195126593f,  -0.6440141975f,  -0.5192897331f,  -0.3697654077f,
	-0.07927779647f, 0.4187757321f,   -0.750078731f,  0.6579554632f,  -0.6859803838f, -0.6878407087f,  0.9490848347f,   0.5795829433f,  -0.5325976529f,  -0.1363699466f,   0.417665879f,	-0.9108236468f, 0.4438605427f,  0.819294887f,	-0.4033873915f,  -0.2817317705f,
	0.3969665622f,   0.5323450134f,   -0.6833017297f, 0.3881436661f,  -0.7119144767f, -0.2306979838f,  -0.9398873022f,  0.1701906676f,  -0.4261839496f,  -0.003712295499f, -0.734675004f,   -0.3195046015f, 0.7345307424f,  0.9766246496f,   -0.02003735175f, -0.4824156342f,
	0.4245892007f,   0.9072427669f,   0.593346808f,   -0.8911762541f, -0.7657571834f, -0.5268198896f,  -0.8801903279f,  -0.6296409617f, -0.09492481344f, -0.4920470525f,   0.7307666154f,   -0.2514540636f, -0.3356210347f, -0.3522787894f,  0.87847885f,	 -0.7424096346f,
	0.5757585274f,   0.4519299338f,   0.6420368628f,  -0.1128478447f, 0.499874883f,   0.5291681739f,   -0.5098837195f,  0.5639583502f,  -0.8456386526f,  -0.9657134875f,   -0.576437342f,   -0.5666013014f, 0.5667702405f,  -0.481316582f,   0.7313389916f,   -0.3805628566f,
	-0.6512675909f,  -0.2787156951f,  0.8648059114f,  -0.9730216276f, -0.8335820906f, 0.2673159641f,   0.231150148f,	0.01286214638f, 0.6774953261f,   0.6542885718f,	-0.02545450161f, 0.2101238586f,  -0.5572105885f, 0.813705672f,	-0.7546026951f,  -0.2502500006f,
	-0.9979289381f,  0.7024037039f,   0.08990874624f, 0.8170812432f,  0.4226980265f,  -0.2442153475f,  -0.9183326731f,  0.6068222411f,  0.818676691f,	-0.7236735282f,   -0.5383903295f,  -0.6269337242f, -0.0939331121f, 0.9203878539f,   -0.7256396824f,  0.6292431149f,
	0.4234156978f,   0.006685688024f, -0.2598694113f, 0.6408036421f,  0.05899871622f, 0.7090281418f,   -0.5905222072f,  0.3128214264f,  -0.691925826f,   0.3634019349f,	-0.6772511147f,  -0.3204583896f, -0.3906740409f, -0.3342190395f,  -0.517779592f,   -0.6817711267f,
	0.6422383105f,   0.4388482478f,   0.2968562611f,  -0.2019778353f, 0.6014865048f,  0.9519280722f,   0.3398889569f,   0.8179709354f,  0.2365522154f,   0.3262175096f,	-0.8060715954f,  -0.2068642503f, 0.6208057279f,  -0.5274282502f,  -0.3722334928f,  -0.8923412971f,
	0.5341834201f,   -0.3663701513f,  -0.6114600319f, 0.5026307556f,  0.8396151729f,  0.9245042467f,   -0.7994843957f,  -0.5357200589f, -0.6283359739f,  -0.61351886f,	 -0.875632008f,   -0.5278879423f, 0.9087491985f,  -0.03500215466f, -0.261365798f,   -0.579523541f,
	-0.3765052689f,  -0.74398252f,	0.4257318052f,  -0.1214508921f, 0.8561809753f,  0.6802835104f,   -0.5452131039f,  -0.1997156478f, 0.4562348357f,   -0.811704301f,	0.67793962f,		-0.9237819106f, 0.6973511259f,  -0.5189506f,	 0.5517320032f,   -0.396710831f,
	0.5493762815f,   -0.2507853002f,  0.4788634005f,  0.387333516f,   -0.2176515694f, 0.6749832419f,   0.2148283022f,   -0.7521815872f, 0.4697000159f,   0.7890593699f,	-0.7606162952f,  0.01083397843f, 0.5254091908f,  -0.6748025877f,  0.751091524f,	0.05259056135f,
	0.01889481232f,  -0.6037423727f,  -0.6542965129f, 0.08873301081f, -0.6191345671f, 0.4331858488f,   -0.3858351946f,  -0.1429059747f, 0.4118221036f,   -0.6247153214f,   -0.611423014f,   0.5542939606f,  -0.9432768808f, -0.4567870451f,  -0.7349133547f,  0.399304489f,
	-0.7474927672f,  0.02589419753f,  0.783915821f,   0.6138668752f,  0.4276376047f,  -0.4347886353f,  0.02947841302f,  -0.833742746f,  0.3817221742f,   -0.8743368359f,   -0.3823443796f,  -0.6829243811f, -0.3681903049f, -0.367626833f,   -0.434583373f,   0.235891995f,
	-0.6874880269f,  -0.5115661773f,  -0.5534962601f, 0.5632777056f,  0.686191532f,   -0.05095871588f, -0.06865785057f, -0.5975288531f, -0.6429790056f,  -0.3729361548f,   0.2237917666f,   0.6046773225f,  -0.5041542295f, -0.03972191174f, 0.7028828406f,   -0.5560856498f,
	0.5898328456f,   -0.9308076766f,  0.4617069864f,  0.3190983137f,  0.9116567753f,  -0.45029554f,	0.3346334459f,   0.8525005645f,  0.2528483381f,   -0.8306630147f,   -0.6880390622f,  0.7448684026f,  -0.1963355843f, -0.5900257974f,  0.9097057294f,   -0.2509196808f,
};
static const float CELL_3D_Y[] = {
	-0.6760585049f, -0.09136176499f, 0.1681325679f,   -0.6688468686f,   -0.4822753902f, -0.7891068824f, -0.1877509944f,  0.548470914f,   -0.463339443f,   -0.4050542082f,  0.3218158513f,  0.2546493823f,  -0.3753271935f, 0.4745384887f,   0.481254652f,   -0.8934416489f,
	-0.6737085076f, 0.7469917228f,   0.3826230411f,   0.6751013678f,	-0.7248119515f, -0.3224276742f, -0.02076190936f, -0.6404268166f, -0.5292028444f,  0.7151414636f,   -0.6144655059f, -0.369912124f,  0.6942067212f,  -0.4481558248f,  -0.6366894559f, 0.5956568471f,
	0.564274539f,   0.7145584688f,   0.6871918316f,   0.5657918509f,	-0.6275978114f, 0.4146983062f,  0.2638993789f,   -0.792633138f,  0.5706133514f,   0.8606546462f,   0.6490900316f,  -0.8242699196f, 0.6765819124f,  0.1959534069f,   -0.8426769757f, -0.5917672797f,
	0.7517364266f,  0.03252559226f,  0.0883617105f,   0.4475064813f,	-0.1418643552f, 0.7343428473f,  0.3870192548f,   -0.7716703522f, 0.4839898327f,   0.7437439055f,   -0.5989573348f, -0.8357068955f, 0.6086049038f,  0.9194627258f,   0.4718297238f,  -0.2650335884f,
	-0.6470352599f, -0.5555181303f,  0.1222351235f,   0.7802044684f,	-0.8636947022f, -0.2341352163f, 0.683030874f,	-0.5005858287f, 0.2334616211f,   0.2576877608f,   0.6666816727f,  -0.7663996863f, 0.794201982f,   0.6189308788f,   0.6071033261f,  -0.4206058253f,
	-0.3957336915f, -0.8170257484f,  -0.1043240417f,  0.0002167596213f, 0.1816339018f,  -0.6838094939f, -0.2495341969f,  -0.7116756954f, -0.03361673621f, -0.3350836431f,  0.2137186039f,  0.2557996786f,  0.7490117093f,  0.4942936549f,   -0.352686853f,  -0.3952445435f,
	-0.0459964767f, -0.7115787471f,  0.08022899756f,  0.5362268157f,	-0.8258613686f, 0.1114171723f,  0.3882823051f,   -0.7915404457f, 0.3250957662f,   0.6401346464f,   -0.2662724517f, -0.6727907114f, -0.994730818f,  -0.3596358977f,  0.2344610069f,  -0.6645215546f,
	-0.7107590611f, -0.4646617327f,  0.6717191355f,   0.5101893498f,	0.1185768238f,  0.236005093f,   -0.7811024061f,  0.5089325193f,  0.6073187658f,   -0.7930732557f,  -0.6822767155f, 0.3201532885f,  0.7545302807f,  0.1072664448f,   0.6784033173f,  -0.6595924967f,
	0.7276509498f,  0.5586689436f,   -0.6498636788f,  0.6789333174f,	0.7105966551f,  -0.2872214155f, 0.496746217f,	-0.3880337977f, 0.7324070604f,   -0.9326634749f,  -0.5867839255f, 0.8003043651f,  -0.1631882481f, -0.6796374681f,  -0.8066678503f, 0.4238177418f,
	0.7715863549f,  0.5455367347f,   -0.03205115397f, -0.6005545066f,   -0.5423640002f, 0.3569205906f,  -0.582071752f,   0.6407354361f,  0.7777142984f,   -0.09956428618f, 0.1100002681f,  0.8136349123f,  0.2923431904f,  0.9735794425f,   0.8324974864f,  -0.6179617717f,
	-0.9248386523f, -0.6448780771f,  -0.5274402761f,  -0.7862170565f,   0.2682099744f,  -0.5848777694f, -0.6364561467f,  -0.7167402514f, -0.8677012494f,  0.4205286707f,   -0.7007832749f, 0.243272451f,   -0.1899846085f, -0.6146124977f,  -0.8093357692f, -0.03545096987f,
	-0.7191590868f, 0.7478645848f,   0.3623517328f,   0.8436992512f,	-0.2445711729f, 0.6897356637f,  -0.1708070787f,  0.4639272368f,  -0.7917186656f,  0.02980025428f,  0.6334156172f,  -0.9815544807f, -0.2307217304f, 0.1080823318f,   0.5167601798f,  -0.845120016f,
	0.441572562f,   0.5876789172f,   -0.6365908737f,  0.68350166f,		0.5849723959f,  0.1164114357f,  -0.7379813884f,  -0.9613237178f, -0.9071943084f,  -0.7682111105f,  0.639074459f,   -0.619358298f,  0.2807257131f,  -0.01800868791f, 0.3776607289f,  0.7207567823f,
	0.5536661486f,  -0.9974053117f,  -0.02047200006f, -0.6739453804f,   -0.5607471297f, 0.8815553192f,  0.8275977415f,   0.3928902456f,  0.550991396f,	0.4247623676f,   -0.3436948871f, -0.3653537677f, 0.3181702902f,  -0.6067173171f,  -0.8984128477f, 0.4220839766f,
	0.7238407199f,  -0.7766913695f,  0.6460037842f,   0.2544775664f,	0.6488840578f,  0.805016833f,   -0.9183807036f,  0.4144046357f,  0.270587208f,	-0.8813684494f,  0.6985971877f,  -0.7795603017f, -0.8624480731f, 0.5532697017f,   0.711179521f,   -0.7798160574f,
	0.5225859041f,  0.1261859368f,   0.3398033582f,   -0.7472173667f,   -0.4032647119f, -0.4246578154f, 0.8481212377f,   -0.2144838537f, 0.3431714491f,   0.5310188231f,   0.6682978632f,  0.3110433206f,  0.9263293599f,  -0.6155600569f,  0.07169784399f, 0.8985888773f,
};
static const float CELL_3D_Z[] = {
	-0.6341391283f,  -0.7207118346f,  0.9597866014f,  0.3237504235f,   -0.7588642466f, -0.01782410481f, 0.2515593809f,   0.2207257205f,   -0.8579541106f, 0.3406410681f,  0.7669470462f,   -0.9431957648f, 0.7676171537f,  -0.6000491115f, -0.7062096948f,  0.2550207115f,
	0.7347325213f,   0.5163625202f,   -0.5394270162f, 0.3336656285f,   -0.0638635111f, -0.6503195787f,  0.3143356798f,   -0.5039217245f,  0.6605180464f,  -0.6855479011f, -0.6693185756f,  0.1832083647f,  -0.5666258437f, 0.3576482138f,  -0.6571949095f,  -0.7522101635f,
	-0.7238865886f,  0.4538887323f,   0.2467106257f,  0.7274778869f,   0.3151170655f,  -0.8802293764f,  -0.2167232729f,  0.5854637865f,   0.7019741052f,  0.5091756071f,  0.1973189533f,   0.46743546f,	0.05197599597f, 0.088354718f,   0.5380464843f,   -0.6458224544f,
	-0.5045952393f,  0.419347884f,	0.8000823542f,  -0.07445020656f, -0.6272881641f, -0.428020311f,   -0.2747382083f,  -0.08987283726f, 0.8699098354f,  0.4524761885f,  -0.3274603257f,  0.4882262167f,  -0.7189983256f, 0.1746079907f,  0.0751772698f,   -0.6152927202f,
	0.4998474673f,   -0.6979677227f,  0.7568667263f,  -0.6152612058f,  0.06447140991f, -0.8155744872f,  -0.5229602449f,  0.6567836838f,   -0.4799905631f, 0.03153534591f, 0.4724992466f,   -0.3026458097f, -0.2191225827f, -0.620692287f,  0.3107552588f,   0.8235670294f,
	0.6474915988f,   -0.5047637941f,  0.4911488878f,  -0.2307138167f,  -0.5216800015f, 0.6789305939f,   0.9403734863f,   0.702390397f,	0.7347584625f,  0.6779567958f,  0.9765635805f,   -0.9436177661f, -0.358465925f,  -0.3058706624f, 0.5533414464f,   -0.8838306897f,
	0.04496841812f,  0.01687374963f,  -0.9927133148f, -0.211752318f,   0.3732015249f,  0.9632990593f,   -0.07682417004f, -0.07232213047f, 0.4733721775f,  0.2579229713f,  0.7995216286f,   0.3928189967f,  0.04107517667f, 0.1534542912f,  0.6468965045f,   0.4030684878f,
	-0.5617300988f,  -0.885463029f,   0.693729985f,   -0.5736527866f,  -0.9911905409f, -0.66451538f,	0.2028855685f,   0.8019541421f,   -0.3903877149f, -0.4888495114f, -0.2753714057f,  -0.8915202143f, 0.5273119089f,  0.9363714773f,  -0.5212228249f,  -0.31642672f,
	0.2409440761f,   -0.703776404f,   -0.6996810411f, -0.7058714505f,  -0.3650566783f, 0.1064744278f,   0.7985729102f,   0.424680257f,	-0.6384535592f, 0.1540161646f,  -0.07702731943f, -0.5627789132f, -0.7667919169f, -0.509815999f,  0.4590525092f,   0.1552595611f,
	0.345402042f,	0.7537656024f,   0.7906259247f,  -0.6218493452f,  0.02979350071f, -0.1337893489f,  -0.1483818606f,  0.549965562f,	0.01882482408f, -0.7833783002f, 0.4702855809f,   0.2435827372f,  0.2978428332f,  0.2256499906f,  0.4885036897f,   0.5312962584f,
	0.05401156992f,  0.1749922158f,   -0.7352273018f, 0.6058980284f,   0.4416079111f,  0.4417378638f,   0.5455879807f,   -0.6681295324f,  0.1973431441f,  0.4053292055f,  0.2220375492f,   0.2957118467f,  0.6910913512f,  0.5940890106f,  -0.2014135283f,  -0.9172588213f,
	-0.4254361401f,  -0.6146586825f,  -0.7996193253f, -0.3716777111f,  -0.9448876842f, -0.2620349924f,  0.9615995749f,   -0.4679683524f,  0.3905937144f,  0.613593722f,   0.1422937358f,   0.1908754211f,  0.8189704912f,  -0.7300408736f, -0.4108776451f,  -0.5319834504f,
	-0.8970265651f,  -0.5386359045f,  0.4082255906f,  0.7245356676f,   0.5239080873f,  -0.8937552226f,  -0.553637673f,   0.2354455182f,   -0.0860293075f, -0.1399373318f, -0.4666323327f,  0.5560157407f,  0.1772619533f,  -0.8893937725f, -0.5632714576f,  -0.5666264959f,
	-0.3670263736f,  -0.06717242579f, 0.6205295181f,  -0.4110536264f,  0.7090054553f,  0.183899597f,	-0.5605470555f,  0.3879565548f,   0.7420893903f,  -0.2347595118f, -0.8577217497f,  0.6325590203f,  -0.8736152276f, 0.7048011129f,  -0.06317948268f, 0.8753285574f,
	-0.05843650473f, -0.3674922622f,  -0.5256624401f, 0.7861039337f,   0.3287714416f,  0.5910593099f,   -0.3896960134f,  0.6864605361f,   0.7164918431f,  -0.290014277f,  -0.6796169617f,  0.1632515592f,  0.04485347486f, 0.8320545697f,  0.01339408056f,  -0.2874989857f,
	0.615630723f,	0.3430367014f,   0.8193658136f,  -0.5829600957f,  0.07911697781f, 0.7854296063f,   -0.4107442306f,  0.4766964066f,   -0.9045999527f, -0.1673856787f, 0.2828077348f,   -0.5902737632f, -0.321506229f,  -0.5224513133f, -0.4090169985f,  -0.3599685311f,
};

Noise::Noise(const int32_t& seed, const float& frequency, const Interp& interp, const Type& type, const int32_t& octaves, const float& lacunarity, const float& gain, const Fractal& fractal, const float& fractalBounding)
	: m_seed(seed), m_perm(std::unique_ptr<uint8_t[]>(new uint8_t[512])), m_perm12(std::unique_ptr<uint8_t[]>(new uint8_t[512])), m_frequency(frequency), m_interp(interp), m_type(type), m_octaves(octaves), m_lacunarity(lacunarity), m_gain(gain), m_fractal(fractal),
	  m_fractalBounding(fractalBounding), m_cellularDistance(CellularDistance::Euclidean), m_cellularReturn(CellularReturn::CellValue), m_cellularNoiseLookup(nullptr), m_cellularDistanceIndex0(0), m_cellularDistanceIndex1(1), m_cellularJitter(0.45f), m_gradientPerturbAmp(1.0f)
{
	SetSeed(seed);
	CalculateFractalBounding();
}

void Noise::SetSeed(const int32_t& seed)
{
	m_seed = seed;

	std::mt19937_64 gen(seed);

	for(int32_t i = 0; i < 256; i++)
		{
			m_perm[i] = static_cast<uint8_t>(i);
		}

	for(int32_t j = 0; j < 256; j++)
		{
			auto rng = static_cast<int32_t>(gen() % (256 - j));
			int32_t k = rng + j;
			int32_t l = m_perm[j];
			m_perm[j] = m_perm[j + 256] = m_perm[k];
			m_perm[k] = static_cast<uint8_t>(l);
			m_perm12[j] = m_perm12[j + 256] = static_cast<uint8_t>(m_perm[j] % 12);
		}
}

void Noise::SetFractalOctaves(const int32_t& octaves)
{
	m_octaves = octaves;
	CalculateFractalBounding();
}

void Noise::SetFractalGain(const float& gain)
{
	m_gain = gain;
	CalculateFractalBounding();
}

void Noise::GetCellularDistance2Indices(int32_t& cellularDistanceIndex0, int32_t& cellularDistanceIndex1) const
{
	cellularDistanceIndex0 = m_cellularDistanceIndex0;
	cellularDistanceIndex1 = m_cellularDistanceIndex1;
}

void Noise::SetCellularDistance2Indices(const int32_t& cellularDistanceIndex0, const int32_t& cellularDistanceIndex1)
{
	m_cellularDistanceIndex0 = std::min(cellularDistanceIndex0, cellularDistanceIndex1);
	m_cellularDistanceIndex1 = std::max(cellularDistanceIndex0, cellularDistanceIndex1);

	m_cellularDistanceIndex0 = std::min(std::max(m_cellularDistanceIndex0, 0), FN_CELLULAR_INDEX_MAX);
	m_cellularDistanceIndex1 = std::min(std::max(m_cellularDistanceIndex1, 0), FN_CELLULAR_INDEX_MAX);
}

// 2D
float Noise::GetValue(float x, float y) const
{
	return SingleValue(0, x * m_frequency, y * m_frequency);
}

float Noise::GetValueFractal(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleValueFractalFbm(x, y);
			case Fractal::Billow:
				return SingleValueFractalBillow(x, y);
			case Fractal::RigidMulti:
				return SingleValueFractalRigidMulti(x, y);
			default:
				return 0.0f;
		}
}

float Noise::GetPerlin(float x, float y) const
{
	return SinglePerlin(0, x * m_frequency, y * m_frequency);
}

float Noise::GetPerlinFractal(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SinglePerlinFractalFbm(x, y);
			case Fractal::Billow:
				return SinglePerlinFractalBillow(x, y);
			case Fractal::RigidMulti:
				return SinglePerlinFractalRigidMulti(x, y);
			default:
				return 0.0f;
		}
}

float Noise::GetSimplex(float x, float y) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency);
}

float Noise::GetSimplexFractal(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleSimplexFractalFbm(x, y);
			case Fractal::Billow:
				return SingleSimplexFractalBillow(x, y);
			case Fractal::RigidMulti:
				return SingleSimplexFractalRigidMulti(x, y);
			default:
				return 0.0f;
		}
}

float Noise::GetCellular(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_cellularReturn)
		{
			case CellularReturn::CellValue:
			case CellularReturn::NoiseLookup:
			case CellularReturn::Distance:
				return SingleCellular(x, y);
			default:
				return SingleCellular2Edge(x, y);
		}
}

float Noise::GetWhiteNoise(float x, float y) const
{
	return ValueCoord2d(m_seed, *reinterpret_cast<int32_t*>(&x) ^ (*reinterpret_cast<int32_t*>(&x) >> 16), *reinterpret_cast<int32_t*>(&y) ^ (*reinterpret_cast<int32_t*>(&y) >> 16));
}

float Noise::GetWhiteNoiseInt(int32_t x, int32_t y) const
{
	return ValueCoord2d(m_seed, x, y);
}

float Noise::GetCubic(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	return SingleCubic(0, x, y);
}

float Noise::GetCubicFractal(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleCubicFractalFbm(x, y);
			case Fractal::Billow:
				return SingleCubicFractalBillow(x, y);
			case Fractal::RigidMulti:
				return SingleCubicFractalRigidMulti(x, y);
			default:
				return 0.0f;
		}
}

float Noise::GetNoise(float x, float y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch(m_type)
		{
			case Type::Value:
				return SingleValue(0, x, y);
			case Type::ValueFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleValueFractalFbm(x, y);
						case Fractal::Billow:
							return SingleValueFractalBillow(x, y);
						case Fractal::RigidMulti:
							return SingleValueFractalRigidMulti(x, y);
					}
			case Type::Perlin:
				return SinglePerlin(0, x, y);
			case Type::PerlinFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SinglePerlinFractalFbm(x, y);
						case Fractal::Billow:
							return SinglePerlinFractalBillow(x, y);
						case Fractal::RigidMulti:
							return SinglePerlinFractalRigidMulti(x, y);
					}
			case Type::Simplex:
				return SingleSimplex(0, x, y);
			case Type::SimplexFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleSimplexFractalFbm(x, y);
						case Fractal::Billow:
							return SingleSimplexFractalBillow(x, y);
						case Fractal::RigidMulti:
							return SingleSimplexFractalRigidMulti(x, y);
					}
			case Type::Cellular:
				switch(m_cellularReturn)
					{
						case CellularReturn::CellValue:
						case CellularReturn::NoiseLookup:
						case CellularReturn::Distance:
							return SingleCellular(x, y);
						default:
							return SingleCellular2Edge(x, y);
					}
			case Type::WhiteNoise:
				return GetWhiteNoise(x, y);
			case Type::Cubic:
				return SingleCubic(0, x, y);
			case Type::CubicFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleCubicFractalFbm(x, y);
						case Fractal::Billow:
							return SingleCubicFractalBillow(x, y);
						case Fractal::RigidMulti:
							return SingleCubicFractalRigidMulti(x, y);
					}
		}
	return 0.0f;
}

void Noise::GradientPerturb(float& x, float& y) const
{
	SingleGradientPerturb(0, m_gradientPerturbAmp, m_frequency, x, y);
}

void Noise::GradientPerturbFractal(float& x, float& y) const
{
	float amp = m_gradientPerturbAmp * m_fractalBounding;
	float freq = m_frequency;
	int32_t i = 0;

	SingleGradientPerturb(m_perm[0], amp, m_frequency, x, y);

	while(++i < m_octaves)
		{
			freq *= m_lacunarity;
			amp *= m_gain;
			SingleGradientPerturb(m_perm[i], amp, freq, x, y);
		}
}

// 3D
float Noise::GetValue(float x, float y, float z) const
{
	return SingleValue(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float Noise::GetValueFractal(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleValueFractalFbm(x, y, z);
			case Fractal::Billow:
				return SingleValueFractalBillow(x, y, z);
			case Fractal::RigidMulti:
				return SingleValueFractalRigidMulti(x, y, z);
			default:
				return 0.0f;
		}
}

float Noise::GetPerlin(float x, float y, float z) const
{
	return SinglePerlin(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float Noise::GetPerlinFractal(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SinglePerlinFractalFbm(x, y, z);
			case Fractal::Billow:
				return SinglePerlinFractalBillow(x, y, z);
			case Fractal::RigidMulti:
				return SinglePerlinFractalRigidMulti(x, y, z);
			default:
				return 0.0f;
		}
}

float Noise::GetSimplex(float x, float y, float z) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float Noise::GetSimplexFractal(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleSimplexFractalFbm(x, y, z);
			case Fractal::Billow:
				return SingleSimplexFractalBillow(x, y, z);
			case Fractal::RigidMulti:
				return SingleSimplexFractalRigidMulti(x, y, z);
			default:
				return 0.0f;
		}
}

float Noise::GetCellular(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_cellularReturn)
		{
			case CellularReturn::CellValue:
			case CellularReturn::NoiseLookup:
			case CellularReturn::Distance:
				return SingleCellular(x, y, z);
			default:
				return SingleCellular2Edge(x, y, z);
		}
}

float Noise::GetWhiteNoise(float x, float y, float z) const
{
	return ValueCoord3d(m_seed, *reinterpret_cast<int32_t*>(&x) ^ (*reinterpret_cast<int32_t*>(&x) >> 16), *reinterpret_cast<int32_t*>(&y) ^ (*reinterpret_cast<int32_t*>(&y) >> 16), *reinterpret_cast<int32_t*>(&z) ^ (*reinterpret_cast<int32_t*>(&z) >> 16));
}

float Noise::GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z) const
{
	return ValueCoord3d(m_seed, x, y, z);
}

float Noise::GetCubic(float x, float y, float z) const
{
	return SingleCubic(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float Noise::GetCubicFractal(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_fractal)
		{
			case Fractal::FBM:
				return SingleCubicFractalFbm(x, y, z);
			case Fractal::Billow:
				return SingleCubicFractalBillow(x, y, z);
			case Fractal::RigidMulti:
				return SingleCubicFractalRigidMulti(x, y, z);
			default:
				return 0.0f;
		}
}

float Noise::GetNoise(float x, float y, float z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch(m_type)
		{
			case Type::Value:
				return SingleValue(0, x, y, z);
			case Type::ValueFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleValueFractalFbm(x, y, z);
						case Fractal::Billow:
							return SingleValueFractalBillow(x, y, z);
						case Fractal::RigidMulti:
							return SingleValueFractalRigidMulti(x, y, z);
						default:
							return 0.0f;
					}
			case Type::Perlin:
				return SinglePerlin(0, x, y, z);
			case Type::PerlinFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SinglePerlinFractalFbm(x, y, z);
						case Fractal::Billow:
							return SinglePerlinFractalBillow(x, y, z);
						case Fractal::RigidMulti:
							return SinglePerlinFractalRigidMulti(x, y, z);
						default:
							return 0.0f;
					}
			case Type::Simplex:
				return SingleSimplex(0, x, y, z);
			case Type::SimplexFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleSimplexFractalFbm(x, y, z);
						case Fractal::Billow:
							return SingleSimplexFractalBillow(x, y, z);
						case Fractal::RigidMulti:
							return SingleSimplexFractalRigidMulti(x, y, z);
						default:
							return 0.0f;
					}
			case Type::Cellular:
				switch(m_cellularReturn)
					{
						case CellularReturn::CellValue:
						case CellularReturn::NoiseLookup:
						case CellularReturn::Distance:
							return SingleCellular(x, y, z);
						default:
							return SingleCellular2Edge(x, y, z);
					}
			case Type::WhiteNoise:
				return GetWhiteNoise(x, y, z);
			case Type::Cubic:
				return SingleCubic(0, x, y, z);
			case Type::CubicFractal:
				switch(m_fractal)
					{
						case Fractal::FBM:
							return SingleCubicFractalFbm(x, y, z);
						case Fractal::Billow:
							return SingleCubicFractalBillow(x, y, z);
						case Fractal::RigidMulti:
							return SingleCubicFractalRigidMulti(x, y, z);
					}
			default:
				return 0.0f;
		}
}

void Noise::GradientPerturb(float& x, float& y, float& z) const
{
	SingleGradientPerturb(0, m_gradientPerturbAmp, m_frequency, x, y, z);
}

void Noise::GradientPerturbFractal(float& x, float& y, float& z) const
{
	float amp = m_gradientPerturbAmp * m_fractalBounding;
	float freq = m_frequency;
	int32_t i = 0;

	SingleGradientPerturb(m_perm[0], amp, m_frequency, x, y, z);

	while(++i < m_octaves)
		{
			freq *= m_lacunarity;
			amp *= m_gain;
			SingleGradientPerturb(m_perm[i], amp, freq, x, y, z);
		}
}

// 4D
float Noise::GetSimplex(float x, float y, float z, float w) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency, w * m_frequency);
}

float Noise::GetWhiteNoise(float x, float y, float z, float w) const
{
	return ValueCoord4d(m_seed, *reinterpret_cast<int32_t*>(&x) ^ (*reinterpret_cast<int32_t*>(&x) >> 16), *reinterpret_cast<int32_t*>(&y) ^ (*reinterpret_cast<int32_t*>(&y) >> 16), *reinterpret_cast<int32_t*>(&z) ^ (*reinterpret_cast<int32_t*>(&z) >> 16),
						*reinterpret_cast<int32_t*>(&w) ^ (*reinterpret_cast<int32_t*>(&w) >> 16));
}

float Noise::GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z, int32_t w) const
{
	return ValueCoord4d(m_seed, x, y, z, w);
}

void Noise::CalculateFractalBounding()
{
	float amp = m_gain;
	float ampFractal = 1.0f;

	for(int32_t i = 1; i < m_octaves; i++)
		{
			ampFractal += amp;
			amp *= m_gain;
		}

	m_fractalBounding = 1.0f / ampFractal;
}

// Helpers
int32_t Noise::FastFloor(const float& f)
{
	return (f >= 0 ? static_cast<int>(f) : static_cast<int>(f) - 1);
}

int32_t Noise::FastRound(const float& f)
{
	return (f >= 0) ? static_cast<int>(f + 0.5f) : static_cast<int>(f - 0.5f);
}

float Noise::Lerp(const float& a, const float& b, const float& t)
{
	return a + t * (b - a);
}

float Noise::InterpHermite(const float& t)
{
	return t * t * (3.0f - 2.0f * t);
}

float Noise::InterpQuintic(const float& t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float Noise::CubicLerp(const float& a, const float& b, const float& c, const float& d, const float& t)
{
	float p = (d - c) - (a - b);
	return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

uint8_t Noise::Index2d12(const uint8_t& offset, const int32_t& x, const int32_t& y) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}

uint8_t Noise::Index3d12(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}

uint8_t Noise::Index4d32(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z, const int32_t& w) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]] & 31;
}

uint8_t Noise::Index2d256(const uint8_t& offset, const int32_t& x, const int32_t& y) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}

uint8_t Noise::Index3d256(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}

uint8_t Noise::Index4d256(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z, const int32_t& w) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]];
}

float Noise::ValueCoord2d(const int32_t& seed, const int32_t& x, const int32_t& y)
{
	int32_t n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	return (n * n * n * 60493) / 2147483648.0f;
}

float Noise::ValueCoord3d(const int32_t& seed, const int32_t& x, const int32_t& y, const int32_t& z)
{
	int32_t n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;

	return (n * n * n * 60493) / 2147483648.0f;
}

float Noise::ValueCoord4d(const int32_t& seed, const int32_t& x, const int32_t& y, const int32_t& z, const int32_t& w)
{
	int32_t n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;
	n ^= W_PRIME * w;
	return (n * n * n * 60493) / 2147483648.0f;
}

float Noise::ValueCoord2dFast(const uint8_t& offset, const int32_t& x, const int32_t& y) const
{
	return VAL_LUT[Index2d256(offset, x, y)];
}

float Noise::ValueCoord3dFast(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z) const
{
	return VAL_LUT[Index3d256(offset, x, y, z)];
}

float Noise::GradCoord2d(const uint8_t& offset, const int32_t& x, const int32_t& y, const float& xd, const float& yd) const
{
	uint8_t lutPos = Index2d12(offset, x, y);
	return xd * GRAD_X[lutPos] + yd * GRAD_Y[lutPos];
}

float Noise::GradCoord3d(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z, const float& xd, const float& yd, const float& zd) const
{
	uint8_t lutPos = Index3d12(offset, x, y, z);
	return xd * GRAD_X[lutPos] + yd * GRAD_Y[lutPos] + zd * GRAD_Z[lutPos];
}

float Noise::GradCoord4d(const uint8_t& offset, const int32_t& x, const int32_t& y, const int32_t& z, const int32_t& w, const float& xd, const float& yd, const float& zd, const float& wd) const
{
	uint8_t lutPos = Index4d32(offset, x, y, z, w) << 2;
	return xd * GRAD_4D[lutPos] + yd * GRAD_4D[lutPos + 1] + zd * GRAD_4D[lutPos + 2] + wd * GRAD_4D[lutPos + 3];
}

// 2D
float Noise::SingleValueFractalFbm(float x, float y) const
{
	float sum = SingleValue(m_perm[0], x, y);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += SingleValue(m_perm[i], x, y) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleValueFractalBillow(float x, float y) const
{
	float sum = std::fabs(SingleValue(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			amp *= m_gain;
			sum += (std::fabs(SingleValue(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleValueFractalRigidMulti(float x, float y) const
{
	float sum = 1.0f - std::fabs(SingleValue(m_perm[0], x, y));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleValue(m_perm[i], x, y))) * amp;
		}

	return sum;
}

float Noise::SingleValue(const uint8_t& offset, const float& x, const float& y) const
{
	int32_t x0 = FastFloor(x);
	int32_t y0 = FastFloor(y);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;

	float xs = 0.0f;
	float ys = 0.0f;

	switch(m_interp)
		{
			case Interp::Linear:
				xs = x - static_cast<float>(x0);
				ys = y - static_cast<float>(y0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(x - static_cast<float>(x0));
				ys = InterpHermite(y - static_cast<float>(y0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(x - static_cast<float>(x0));
				ys = InterpQuintic(y - static_cast<float>(y0));
				break;
		}

	float xf0 = Lerp(ValueCoord2dFast(offset, x0, y0), ValueCoord2dFast(offset, x1, y0), xs);
	float xf1 = Lerp(ValueCoord2dFast(offset, x0, y1), ValueCoord2dFast(offset, x1, y1), xs);

	return Lerp(xf0, xf1, ys);
}

float Noise::SinglePerlinFractalFbm(float x, float y) const
{
	float sum = SinglePerlin(m_perm[0], x, y);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += SinglePerlin(m_perm[i], x, y) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SinglePerlinFractalBillow(float x, float y) const
{
	float sum = std::fabs(SinglePerlin(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SinglePerlin(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SinglePerlinFractalRigidMulti(float x, float y) const
{
	float sum = 1.0f - std::fabs(SinglePerlin(m_perm[0], x, y));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SinglePerlin(m_perm[i], x, y))) * amp;
		}

	return sum;
}

float Noise::SinglePerlin(const uint8_t& offset, const float& x, const float& y) const
{
	int32_t x0 = FastFloor(x);
	int32_t y0 = FastFloor(y);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;

	float xs = 0.0f;
	float ys = 0.0f;

	switch(m_interp)
		{
			case Interp::Linear:
				xs = x - static_cast<float>(x0);
				ys = y - static_cast<float>(y0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(x - static_cast<float>(x0));
				ys = InterpHermite(y - static_cast<float>(y0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(x - static_cast<float>(x0));
				ys = InterpQuintic(y - static_cast<float>(y0));
				break;
		}

	float xd0 = x - static_cast<float>(x0);
	float yd0 = y - static_cast<float>(y0);
	float xd1 = xd0 - 1.0f;
	float yd1 = yd0 - 1.0f;

	float xf0 = Lerp(GradCoord2d(offset, x0, y0, xd0, yd0), GradCoord2d(offset, x1, y0, xd1, yd0), xs);
	float xf1 = Lerp(GradCoord2d(offset, x0, y1, xd0, yd1), GradCoord2d(offset, x1, y1, xd1, yd1), xs);

	return Lerp(xf0, xf1, ys);
}

float Noise::SingleSimplexFractalFbm(float x, float y) const
{
	float sum = SingleSimplex(m_perm[0], x, y);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += SingleSimplex(m_perm[i], x, y) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleSimplexFractalBillow(float x, float y) const
{
	float sum = std::fabs(SingleSimplex(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SingleSimplex(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleSimplexFractalRigidMulti(float x, float y) const
{
	float sum = 1.0f - std::fabs(SingleSimplex(m_perm[0], x, y));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleSimplex(m_perm[i], x, y))) * amp;
		}

	return sum;
}

float Noise::singleSimplexFractalBlend(float x, float y) const
{
	float sum = SingleSimplex(m_perm[0], x, y);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum *= SingleSimplex(m_perm[i], x, y) * amp + 1.0f;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleSimplex(const uint8_t& offset, const float& x, const float& y) const
{
	float t = (x + y) * F2;
	int32_t i = FastFloor(x + t);
	int32_t j = FastFloor(y + t);

	t = (i + j) * G2;
	float X0 = i - t;
	float Y0 = j - t;

	float x0 = x - X0;
	float y0 = y - Y0;

	int32_t i1, j1;

	if(x0 > y0)
		{
			i1 = 1;
			j1 = 0;
		}
	else
		{
			i1 = 0;
			j1 = 1;
		}

	float x1 = x0 - static_cast<float>(i1) + G2;
	float y1 = y0 - static_cast<float>(j1) + G2;
	float x2 = x0 - 1.0f + 2.0f * G2;
	float y2 = y0 - 1.0f + 2.0f * G2;

	float n0, n1, n2;

	t = 0.5f - x0 * x0 - y0 * y0;

	if(t < 0.0f)
		{
			n0 = 0.0f;
		}
	else
		{
			t *= t;
			n0 = t * t * GradCoord2d(offset, i, j, x0, y0);
		}

	t = 0.5f - x1 * x1 - y1 * y1;

	if(t < 0.0f)
		{
			n1 = 0.0f;
		}
	else
		{
			t *= t;
			n1 = t * t * GradCoord2d(offset, i + i1, j + j1, x1, y1);
		}

	t = 0.5f - x2 * x2 - y2 * y2;

	if(t < 0.0f)
		{
			n2 = 0.0f;
		}
	else
		{
			t *= t;
			n2 = t * t * GradCoord2d(offset, i + 1, j + 1, x2, y2);
		}

	return 70.0f * (n0 + n1 + n2);
}

float Noise::SingleCubicFractalFbm(float x, float y) const
{
	float sum = SingleCubic(m_perm[0], x, y);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += SingleCubic(m_perm[i], x, y) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleCubicFractalBillow(float x, float y) const
{
	float sum = std::fabs(SingleCubic(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SingleCubic(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleCubicFractalRigidMulti(float x, float y) const
{
	float sum = 1.0f - std::fabs(SingleCubic(m_perm[0], x, y));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleCubic(m_perm[i], x, y))) * amp;
		}

	return sum;
}

float Noise::SingleCubic(const uint8_t& offset, const float& x, const float& y) const
{
	int32_t x1 = FastFloor(x);
	int32_t y1 = FastFloor(y);

	int32_t x0 = x1 - 1;
	int32_t y0 = y1 - 1;
	int32_t x2 = x1 + 1;
	int32_t y2 = y1 + 1;
	int32_t x3 = x1 + 2;
	int32_t y3 = y1 + 2;

	float xs = x - static_cast<float>(x1);
	float ys = y - static_cast<float>(y1);

	return CubicLerp(CubicLerp(ValueCoord2dFast(offset, x0, y0), ValueCoord2dFast(offset, x1, y0), ValueCoord2dFast(offset, x2, y0), ValueCoord2dFast(offset, x3, y0), xs),
					 CubicLerp(ValueCoord2dFast(offset, x0, y1), ValueCoord2dFast(offset, x1, y1), ValueCoord2dFast(offset, x2, y1), ValueCoord2dFast(offset, x3, y1), xs),
					 CubicLerp(ValueCoord2dFast(offset, x0, y2), ValueCoord2dFast(offset, x1, y2), ValueCoord2dFast(offset, x2, y2), ValueCoord2dFast(offset, x3, y2), xs),
					 CubicLerp(ValueCoord2dFast(offset, x0, y3), ValueCoord2dFast(offset, x1, y3), ValueCoord2dFast(offset, x2, y3), ValueCoord2dFast(offset, x3, y3), xs), ys) *
		   CUBIC_2D_BOUNDING;
}

float Noise::SingleCellular(const float& x, const float& y) const
{
	int32_t xr = FastRound(x);
	int32_t yr = FastRound(y);

	float distance = 999999.0f;
	int32_t xc = 0;
	int32_t yc = 0;

	switch(m_cellularDistance)
		{
			default:
			case CellularDistance::Euclidean:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = vecX * vecX + vecY * vecY;

								if(newDistance < distance)
									{
										distance = newDistance;
										xc = xi;
										yc = yi;
									}
							}
					}
				break;
			case CellularDistance::Manhattan:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = (std::fabs(vecX) + std::fabs(vecY));

								if(newDistance < distance)
									{
										distance = newDistance;
										xc = xi;
										yc = yi;
									}
							}
					}
				break;
			case CellularDistance::Natural:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = (std::fabs(vecX) + std::fabs(vecY)) + (vecX * vecX + vecY * vecY);

								if(newDistance < distance)
									{
										distance = newDistance;
										xc = xi;
										yc = yi;
									}
							}
					}
				break;
		}

	uint8_t lutPos;

	switch(m_cellularReturn)
		{
			case CellularReturn::CellValue:
				return ValueCoord2d(m_seed, xc, yc);

			case CellularReturn::NoiseLookup:
				assert(m_cellularNoiseLookup);
				lutPos = Index2d256(0, xc, yc);
				return m_cellularNoiseLookup->GetNoise(xc + CELL_2D_X[lutPos] * m_cellularJitter, yc + CELL_2D_Y[lutPos] * m_cellularJitter);
			case CellularReturn::Distance:
				return distance;
			default:
				return 0.0f;
		}
}

float Noise::SingleCellular2Edge(const float& x, const float& y) const
{
	int32_t xr = FastRound(x);
	int32_t yr = FastRound(y);

	float distance[FN_CELLULAR_INDEX_MAX + 1] = {999999.0f, 999999.0f, 999999.0f, 999999.0f};

	switch(m_cellularDistance)
		{
			default:
			case CellularDistance::Euclidean:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = vecX * vecX + vecY * vecY;

								for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
									{
										distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
									}

								distance[0] = std::fmin(distance[0], newDistance);
							}
					}
				break;
			case CellularDistance::Manhattan:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = std::fabs(vecX) + std::fabs(vecY);

								for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
									{
										distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
									}

								distance[0] = std::fmin(distance[0], newDistance);
							}
					}
				break;
			case CellularDistance::Natural:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								uint8_t lutPos = Index2d256(0, xi, yi);

								float vecX = xi - x + CELL_2D_X[lutPos] * m_cellularJitter;
								float vecY = yi - y + CELL_2D_Y[lutPos] * m_cellularJitter;

								float newDistance = (std::fabs(vecX) + std::fabs(vecY)) + (vecX * vecX + vecY * vecY);

								for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
									{
										distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
									}

								distance[0] = std::fmin(distance[0], newDistance);
							}
					}
				break;
		}

	switch(m_cellularReturn)
		{
			case CellularReturn::Distance2:
				return distance[m_cellularDistanceIndex1];
			case CellularReturn::Distance2Add:
				return distance[m_cellularDistanceIndex1] + distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Sub:
				return distance[m_cellularDistanceIndex1] - distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Mul:
				return distance[m_cellularDistanceIndex1] * distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Div:
				return distance[m_cellularDistanceIndex0] / distance[m_cellularDistanceIndex1];
			default:
				return 0.0f;
		}
}

void Noise::SingleGradientPerturb(const uint8_t& offset, const float& warpAmp, const float& frequency, float x, float y) const
{
	float xf = x * frequency;
	float yf = y * frequency;

	int32_t x0 = FastFloor(xf);
	int32_t y0 = FastFloor(yf);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;

	float xs, ys;

	switch(m_interp)
		{
			default:
			case Interp::Linear:
				xs = xf - static_cast<float>(x0);
				ys = yf - static_cast<float>(y0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(xf - static_cast<float>(x0));
				ys = InterpHermite(yf - static_cast<float>(y0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(xf - static_cast<float>(x0));
				ys = InterpQuintic(yf - static_cast<float>(y0));
				break;
		}

	int32_t lutPos0 = Index2d256(offset, x0, y0);
	int32_t lutPos1 = Index2d256(offset, x1, y0);

	float lx0x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	float ly0x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	lutPos0 = Index2d256(offset, x0, y1);
	lutPos1 = Index2d256(offset, x1, y1);

	float lx1x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	float ly1x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	x += Lerp(lx0x, lx1x, ys) * warpAmp;
	y += Lerp(ly0x, ly1x, ys) * warpAmp;
}

// 3D
float Noise::SingleValueFractalFbm(float x, float y, float z) const
{
	float sum = SingleValue(m_perm[0], x, y, z);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += SingleValue(m_perm[i], x, y, z) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleValueFractalBillow(float x, float y, float z) const
{
	float sum = std::fabs(SingleValue(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SingleValue(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleValueFractalRigidMulti(float x, float y, float z) const
{
	float sum = 1.0f - std::fabs(SingleValue(m_perm[0], x, y, z));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleValue(m_perm[i], x, y, z))) * amp;
		}

	return sum;
}

float Noise::SingleValue(const uint8_t& offset, const float& x, const float& y, const float& z) const
{
	int32_t x0 = FastFloor(x);
	int32_t y0 = FastFloor(y);
	int32_t z0 = FastFloor(z);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;
	int32_t z1 = z0 + 1;

	float xs = 0.0f;
	float ys = 0.0f;
	float zs = 0.0f;

	switch(m_interp)
		{
			case Interp::Linear:
				xs = x - static_cast<float>(x0);
				ys = y - static_cast<float>(y0);
				zs = z - static_cast<float>(z0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(x - static_cast<float>(x0));
				ys = InterpHermite(y - static_cast<float>(y0));
				zs = InterpHermite(z - static_cast<float>(z0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(x - static_cast<float>(x0));
				ys = InterpQuintic(y - static_cast<float>(y0));
				zs = InterpQuintic(z - static_cast<float>(z0));
				break;
		}

	float xf00 = Lerp(ValueCoord3dFast(offset, x0, y0, z0), ValueCoord3dFast(offset, x1, y0, z0), xs);
	float xf10 = Lerp(ValueCoord3dFast(offset, x0, y1, z0), ValueCoord3dFast(offset, x1, y1, z0), xs);
	float xf01 = Lerp(ValueCoord3dFast(offset, x0, y0, z1), ValueCoord3dFast(offset, x1, y0, z1), xs);
	float xf11 = Lerp(ValueCoord3dFast(offset, x0, y1, z1), ValueCoord3dFast(offset, x1, y1, z1), xs);

	float yf0 = Lerp(xf00, xf10, ys);
	float yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

float Noise::SinglePerlinFractalFbm(float x, float y, float z) const
{
	float sum = SinglePerlin(m_perm[0], x, y, z);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += SinglePerlin(m_perm[i], x, y, z) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SinglePerlinFractalBillow(float x, float y, float z) const
{
	float sum = std::fabs(SinglePerlin(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SinglePerlin(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SinglePerlinFractalRigidMulti(float x, float y, float z) const
{
	float sum = 1.0f - std::fabs(SinglePerlin(m_perm[0], x, y, z));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SinglePerlin(m_perm[i], x, y, z))) * amp;
		}

	return sum;
}

float Noise::SinglePerlin(const uint8_t& offset, const float& x, const float& y, const float& z) const
{
	int32_t x0 = FastFloor(x);
	int32_t y0 = FastFloor(y);
	int32_t z0 = FastFloor(z);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;
	int32_t z1 = z0 + 1;

	float xs = 0.0f;
	float ys = 0.0f;
	float zs = 0.0f;

	switch(m_interp)
		{
			case Interp::Linear:
				xs = x - static_cast<float>(x0);
				ys = y - static_cast<float>(y0);
				zs = z - static_cast<float>(z0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(x - static_cast<float>(x0));
				ys = InterpHermite(y - static_cast<float>(y0));
				zs = InterpHermite(z - static_cast<float>(z0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(x - static_cast<float>(x0));
				ys = InterpQuintic(y - static_cast<float>(y0));
				zs = InterpQuintic(z - static_cast<float>(z0));
				break;
		}

	float xd0 = x - static_cast<float>(x0);
	float yd0 = y - static_cast<float>(y0);
	float zd0 = z - static_cast<float>(z0);
	float xd1 = xd0 - 1.0f;
	float yd1 = yd0 - 1.0f;
	float zd1 = zd0 - 1.0f;

	float xf00 = Lerp(GradCoord3d(offset, x0, y0, z0, xd0, yd0, zd0), GradCoord3d(offset, x1, y0, z0, xd1, yd0, zd0), xs);
	float xf10 = Lerp(GradCoord3d(offset, x0, y1, z0, xd0, yd1, zd0), GradCoord3d(offset, x1, y1, z0, xd1, yd1, zd0), xs);
	float xf01 = Lerp(GradCoord3d(offset, x0, y0, z1, xd0, yd0, zd1), GradCoord3d(offset, x1, y0, z1, xd1, yd0, zd1), xs);
	float xf11 = Lerp(GradCoord3d(offset, x0, y1, z1, xd0, yd1, zd1), GradCoord3d(offset, x1, y1, z1, xd1, yd1, zd1), xs);

	float yf0 = Lerp(xf00, xf10, ys);
	float yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

float Noise::SingleSimplexFractalFbm(float x, float y, float z) const
{
	float sum = SingleSimplex(m_perm[0], x, y, z);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += SingleSimplex(m_perm[i], x, y, z) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleSimplexFractalBillow(float x, float y, float z) const
{
	float sum = std::fabs(SingleSimplex(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SingleSimplex(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleSimplexFractalRigidMulti(float x, float y, float z) const
{
	float sum = 1.0f - std::fabs(SingleSimplex(m_perm[0], x, y, z));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleSimplex(m_perm[i], x, y, z))) * amp;
		}

	return sum;
}

float Noise::SingleSimplex(const uint8_t& offset, const float& x, const float& y, const float& z) const
{
	float t = (x + y + z) * F3;
	int32_t i = FastFloor(x + t);
	int32_t j = FastFloor(y + t);
	int32_t k = FastFloor(z + t);

	t = (i + j + k) * G3;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;

	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;

	int32_t i1, j1, k1;
	int32_t i2, j2, k2;

	if(x0 >= y0)
		{
			if(y0 >= z0)
				{
					i1 = 1;
					j1 = 0;
					k1 = 0;
					i2 = 1;
					j2 = 1;
					k2 = 0;
				}
			else if(x0 >= z0)
				{
					i1 = 1;
					j1 = 0;
					k1 = 0;
					i2 = 1;
					j2 = 0;
					k2 = 1;
				}
			else // x0 < z0
				{
					i1 = 0;
					j1 = 0;
					k1 = 1;
					i2 = 1;
					j2 = 0;
					k2 = 1;
				}
		}
	else // x0 < y0
		{
			if(y0 < z0)
				{
					i1 = 0;
					j1 = 0;
					k1 = 1;
					i2 = 0;
					j2 = 1;
					k2 = 1;
				}
			else if(x0 < z0)
				{
					i1 = 0;
					j1 = 1;
					k1 = 0;
					i2 = 0;
					j2 = 1;
					k2 = 1;
				}
			else // x0 >= z0
				{
					i1 = 0;
					j1 = 1;
					k1 = 0;
					i2 = 1;
					j2 = 1;
					k2 = 0;
				}
		}

	float x1 = x0 - i1 + G3;
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0f * G3;
	float y2 = y0 - j2 + 2.0f * G3;
	float z2 = z0 - k2 + 2.0f * G3;
	float x3 = x0 - 1.0f + 3.0f * G3;
	float y3 = y0 - 1.0f + 3.0f * G3;
	float z3 = z0 - 1.0f + 3.0f * G3;

	float n0, n1, n2, n3;

	t = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;

	if(t < 0.0f)
		{
			n0 = 0.0f;
		}
	else
		{
			t *= t;
			n0 = t * t * GradCoord3d(offset, i, j, k, x0, y0, z0);
		}

	t = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;

	if(t < 0.0f)
		{
			n1 = 0.0f;
		}
	else
		{
			t *= t;
			n1 = t * t * GradCoord3d(offset, i + i1, j + j1, k + k1, x1, y1, z1);
		}

	t = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;

	if(t < 0.0f)
		{
			n2 = 0.0f;
		}
	else
		{
			t *= t;
			n2 = t * t * GradCoord3d(offset, i + i2, j + j2, k + k2, x2, y2, z2);
		}

	t = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;

	if(t < 0.0f)
		{
			n3 = 0.0f;
		}
	else
		{
			t *= t;
			n3 = t * t * GradCoord3d(offset, i + 1, j + 1, k + 1, x3, y3, z3);
		}

	return 32.0f * (n0 + n1 + n2 + n3);
}

float Noise::SingleCubicFractalFbm(float x, float y, float z) const
{
	float sum = SingleCubic(m_perm[0], x, y, z);
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += SingleCubic(m_perm[i], x, y, z) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleCubicFractalBillow(float x, float y, float z) const
{
	float sum = std::fabs(SingleCubic(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum += (std::fabs(SingleCubic(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
		}

	return sum * m_fractalBounding;
}

float Noise::SingleCubicFractalRigidMulti(float x, float y, float z) const
{
	float sum = 1.0f - std::fabs(SingleCubic(m_perm[0], x, y, z));
	float amp = 1.0f;
	int32_t i = 0;

	while(++i < m_octaves)
		{
			x *= m_lacunarity;
			y *= m_lacunarity;
			z *= m_lacunarity;

			amp *= m_gain;
			sum -= (1.0f - std::fabs(SingleCubic(m_perm[i], x, y, z))) * amp;
		}

	return sum;
}

float Noise::SingleCubic(const uint8_t& offset, const float& x, const float& y, const float& z) const
{
	int32_t x1 = FastFloor(x);
	int32_t y1 = FastFloor(y);
	int32_t z1 = FastFloor(z);

	int32_t x0 = x1 - 1;
	int32_t y0 = y1 - 1;
	int32_t z0 = z1 - 1;
	int32_t x2 = x1 + 1;
	int32_t y2 = y1 + 1;
	int32_t z2 = z1 + 1;
	int32_t x3 = x1 + 2;
	int32_t y3 = y1 + 2;
	int32_t z3 = z1 + 2;

	float xs = x - static_cast<float>(x1);
	float ys = y - static_cast<float>(y1);
	float zs = z - static_cast<float>(z1);

	return CubicLerp(CubicLerp(CubicLerp(ValueCoord3dFast(offset, x0, y0, z0), ValueCoord3dFast(offset, x1, y0, z0), ValueCoord3dFast(offset, x2, y0, z0), ValueCoord3dFast(offset, x3, y0, z0), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y1, z0), ValueCoord3dFast(offset, x1, y1, z0), ValueCoord3dFast(offset, x2, y1, z0), ValueCoord3dFast(offset, x3, y1, z0), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y2, z0), ValueCoord3dFast(offset, x1, y2, z0), ValueCoord3dFast(offset, x2, y2, z0), ValueCoord3dFast(offset, x3, y2, z0), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y3, z0), ValueCoord3dFast(offset, x1, y3, z0), ValueCoord3dFast(offset, x2, y3, z0), ValueCoord3dFast(offset, x3, y3, z0), xs), ys),
					 CubicLerp(CubicLerp(ValueCoord3dFast(offset, x0, y0, z1), ValueCoord3dFast(offset, x1, y0, z1), ValueCoord3dFast(offset, x2, y0, z1), ValueCoord3dFast(offset, x3, y0, z1), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y1, z1), ValueCoord3dFast(offset, x1, y1, z1), ValueCoord3dFast(offset, x2, y1, z1), ValueCoord3dFast(offset, x3, y1, z1), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y2, z1), ValueCoord3dFast(offset, x1, y2, z1), ValueCoord3dFast(offset, x2, y2, z1), ValueCoord3dFast(offset, x3, y2, z1), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y3, z1), ValueCoord3dFast(offset, x1, y3, z1), ValueCoord3dFast(offset, x2, y3, z1), ValueCoord3dFast(offset, x3, y3, z1), xs), ys),
					 CubicLerp(CubicLerp(ValueCoord3dFast(offset, x0, y0, z2), ValueCoord3dFast(offset, x1, y0, z2), ValueCoord3dFast(offset, x2, y0, z2), ValueCoord3dFast(offset, x3, y0, z2), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y1, z2), ValueCoord3dFast(offset, x1, y1, z2), ValueCoord3dFast(offset, x2, y1, z2), ValueCoord3dFast(offset, x3, y1, z2), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y2, z2), ValueCoord3dFast(offset, x1, y2, z2), ValueCoord3dFast(offset, x2, y2, z2), ValueCoord3dFast(offset, x3, y2, z2), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y3, z2), ValueCoord3dFast(offset, x1, y3, z2), ValueCoord3dFast(offset, x2, y3, z2), ValueCoord3dFast(offset, x3, y3, z2), xs), ys),
					 CubicLerp(CubicLerp(ValueCoord3dFast(offset, x0, y0, z3), ValueCoord3dFast(offset, x1, y0, z3), ValueCoord3dFast(offset, x2, y0, z3), ValueCoord3dFast(offset, x3, y0, z3), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y1, z3), ValueCoord3dFast(offset, x1, y1, z3), ValueCoord3dFast(offset, x2, y1, z3), ValueCoord3dFast(offset, x3, y1, z3), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y2, z3), ValueCoord3dFast(offset, x1, y2, z3), ValueCoord3dFast(offset, x2, y2, z3), ValueCoord3dFast(offset, x3, y2, z3), xs),
							   CubicLerp(ValueCoord3dFast(offset, x0, y3, z3), ValueCoord3dFast(offset, x1, y3, z3), ValueCoord3dFast(offset, x2, y3, z3), ValueCoord3dFast(offset, x3, y3, z3), xs), ys),
					 zs) *
		   CUBIC_3D_BOUNDING;
}

float Noise::SingleCellular(const float& x, const float& y, const float& z) const
{
	int32_t xr = FastRound(x);
	int32_t yr = FastRound(y);
	int32_t zr = FastRound(z);

	float distance = 999999.0f;
	int32_t xc = 0;
	int32_t yc = 0;
	int32_t zc = 0;

	switch(m_cellularDistance)
		{
			case CellularDistance::Euclidean:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

										if(newDistance < distance)
											{
												distance = newDistance;
												xc = xi;
												yc = yi;
												zc = zi;
											}
									}
							}
					}
				break;
			case CellularDistance::Manhattan:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = std::fabs(vecX) + std::fabs(vecY) + std::fabs(vecZ);

										if(newDistance < distance)
											{
												distance = newDistance;
												xc = xi;
												yc = yi;
												zc = zi;
											}
									}
							}
					}
				break;
			case CellularDistance::Natural:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = (std::fabs(vecX) + std::fabs(vecY) + std::fabs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

										if(newDistance < distance)
											{
												distance = newDistance;
												xc = xi;
												yc = yi;
												zc = zi;
											}
									}
							}
					}
				break;
			default:
				break;
		}

	uint8_t lutPos;

	switch(m_cellularReturn)
		{
			case CellularReturn::CellValue:
				return ValueCoord3d(m_seed, xc, yc, zc);
			case CellularReturn::NoiseLookup:
				assert(m_cellularNoiseLookup);

				lutPos = Index3d256(0, xc, yc, zc);
				return m_cellularNoiseLookup->GetNoise(xc + CELL_3D_X[lutPos] * m_cellularJitter, yc + CELL_3D_Y[lutPos] * m_cellularJitter, zc + CELL_3D_Z[lutPos] * m_cellularJitter);
			case CellularReturn::Distance:
				return distance;
			default:
				return 0.0f;
		}
}

float Noise::SingleCellular2Edge(const float& x, const float& y, const float& z) const
{
	int32_t xr = FastRound(x);
	int32_t yr = FastRound(y);
	int32_t zr = FastRound(z);

	float distance[FN_CELLULAR_INDEX_MAX + 1] = {999999.0f, 999999.0f, 999999.0f, 999999.0f};

	switch(m_cellularDistance)
		{
			case CellularDistance::Euclidean:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

										for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
											{
												distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
											}

										distance[0] = std::fmin(distance[0], newDistance);
									}
							}
					}
				break;
			case CellularDistance::Manhattan:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = std::fabs(vecX) + std::fabs(vecY) + std::fabs(vecZ);

										for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
											{
												distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
											}

										distance[0] = std::fmin(distance[0], newDistance);
									}
							}
					}
				break;
			case CellularDistance::Natural:
				for(int32_t xi = xr - 1; xi <= xr + 1; xi++)
					{
						for(int32_t yi = yr - 1; yi <= yr + 1; yi++)
							{
								for(int32_t zi = zr - 1; zi <= zr + 1; zi++)
									{
										uint8_t lutPos = Index3d256(0, xi, yi, zi);

										float vecX = xi - x + CELL_3D_X[lutPos] * m_cellularJitter;
										float vecY = yi - y + CELL_3D_Y[lutPos] * m_cellularJitter;
										float vecZ = zi - z + CELL_3D_Z[lutPos] * m_cellularJitter;

										float newDistance = (std::fabs(vecX) + std::fabs(vecY) + std::fabs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

										for(int32_t i = m_cellularDistanceIndex1; i > 0; i--)
											{
												distance[i] = std::fmax(std::fmin(distance[i], newDistance), distance[i - 1]);
											}

										distance[0] = std::fmin(distance[0], newDistance);
									}
							}
					}
				break;
			default:
				break;
		}

	switch(m_cellularReturn)
		{
			case CellularReturn::Distance2:
				return distance[m_cellularDistanceIndex1];
			case CellularReturn::Distance2Add:
				return distance[m_cellularDistanceIndex1] + distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Sub:
				return distance[m_cellularDistanceIndex1] - distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Mul:
				return distance[m_cellularDistanceIndex1] * distance[m_cellularDistanceIndex0];
			case CellularReturn::Distance2Div:
				return distance[m_cellularDistanceIndex0] / distance[m_cellularDistanceIndex1];
			default:
				return 0.0f;
		}
}

void Noise::SingleGradientPerturb(const uint8_t& offset, const float& warpAmp, const float& frequency, float x, float y, float z) const
{
	float xf = x * frequency;
	float yf = y * frequency;
	float zf = z * frequency;

	int32_t x0 = FastFloor(xf);
	int32_t y0 = FastFloor(yf);
	int32_t z0 = FastFloor(zf);
	int32_t x1 = x0 + 1;
	int32_t y1 = y0 + 1;
	int32_t z1 = z0 + 1;

	float xs, ys, zs;

	switch(m_interp)
		{
			default:
			case Interp::Linear:
				xs = xf - static_cast<float>(x0);
				ys = yf - static_cast<float>(y0);
				zs = zf - static_cast<float>(z0);
				break;
			case Interp::Hermite:
				xs = InterpHermite(xf - static_cast<float>(x0));
				ys = InterpHermite(yf - static_cast<float>(y0));
				zs = InterpHermite(zf - static_cast<float>(z0));
				break;
			case Interp::Quintic:
				xs = InterpQuintic(xf - static_cast<float>(x0));
				ys = InterpQuintic(yf - static_cast<float>(y0));
				zs = InterpQuintic(zf - static_cast<float>(z0));
				break;
		}

	int32_t lutPos0 = Index3d256(offset, x0, y0, z0);
	int32_t lutPos1 = Index3d256(offset, x1, y0, z0);

	float lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	float ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	float lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3d256(offset, x0, y1, z0);
	lutPos1 = Index3d256(offset, x1, y1, z0);

	float lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	float ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	float lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	float lx0y = Lerp(lx0x, lx1x, ys);
	float ly0y = Lerp(ly0x, ly1x, ys);
	float lz0y = Lerp(lz0x, lz1x, ys);

	lutPos0 = Index3d256(offset, x0, y0, z1);
	lutPos1 = Index3d256(offset, x1, y0, z1);

	lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3d256(offset, x0, y1, z1);
	lutPos1 = Index3d256(offset, x1, y1, z1);

	lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	x += Lerp(lx0y, Lerp(lx0x, lx1x, ys), zs) * warpAmp;
	y += Lerp(ly0y, Lerp(ly0x, ly1x, ys), zs) * warpAmp;
	z += Lerp(lz0y, Lerp(lz0x, lz1x, ys), zs) * warpAmp;
}

// 4D
float Noise::SingleSimplex(const uint8_t& offset, const float& x, const float& y, const float& z, const float& w) const
{
	float n0, n1, n2, n3, n4;
	float t = (x + y + z + w) * F4;
	int32_t i = FastFloor(x + t);
	int32_t j = FastFloor(y + t);
	int32_t k = FastFloor(z + t);
	int32_t l = FastFloor(w + t);
	t = (i + j + k + l) * G4;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;
	float W0 = l - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;
	float w0 = w - W0;

	int32_t rankx = 0;
	int32_t ranky = 0;
	int32_t rankz = 0;
	int32_t rankw = 0;

	if(x0 > y0)
		{
			rankx++;
		}
	else
		{
			ranky++;
		}

	if(x0 > z0)
		{
			rankx++;
		}
	else
		{
			rankz++;
		}

	if(x0 > w0)
		{
			rankx++;
		}
	else
		{
			rankw++;
		}

	if(y0 > z0)
		{
			ranky++;
		}
	else
		{
			rankz++;
		}

	if(y0 > w0)
		{
			ranky++;
		}
	else
		{
			rankw++;
		}

	if(z0 > w0)
		{
			rankz++;
		}
	else
		{
			rankw++;
		}

	int32_t i1 = rankx >= 3 ? 1 : 0;
	int32_t j1 = ranky >= 3 ? 1 : 0;
	int32_t k1 = rankz >= 3 ? 1 : 0;
	int32_t l1 = rankw >= 3 ? 1 : 0;

	int32_t i2 = rankx >= 2 ? 1 : 0;
	int32_t j2 = ranky >= 2 ? 1 : 0;
	int32_t k2 = rankz >= 2 ? 1 : 0;
	int32_t l2 = rankw >= 2 ? 1 : 0;

	int32_t i3 = rankx >= 1 ? 1 : 0;
	int32_t j3 = ranky >= 1 ? 1 : 0;
	int32_t k3 = rankz >= 1 ? 1 : 0;
	int32_t l3 = rankw >= 1 ? 1 : 0;

	float x1 = x0 - i1 + G4;
	float y1 = y0 - j1 + G4;
	float z1 = z0 - k1 + G4;
	float w1 = w0 - l1 + G4;
	float x2 = x0 - i2 + 2.0f * G4;
	float y2 = y0 - j2 + 2.0f * G4;
	float z2 = z0 - k2 + 2.0f * G4;
	float w2 = w0 - l2 + 2.0f * G4;
	float x3 = x0 - i3 + 3.0f * G4;
	float y3 = y0 - j3 + 3.0f * G4;
	float z3 = z0 - k3 + 3.0f * G4;
	float w3 = w0 - l3 + 3.0f * G4;
	float x4 = x0 - 1.0f + 4.0f * G4;
	float y4 = y0 - 1.0f + 4.0f * G4;
	float z4 = z0 - 1.0f + 4.0f * G4;
	float w4 = w0 - 1.0f + 4.0f * G4;

	t = 0.6f - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;

	if(t < 0.0f)
		{
			n0 = 0.0f;
		}
	else
		{
			t *= t;
			n0 = t * t * GradCoord4d(offset, i, j, k, l, x0, y0, z0, w0);
		}

	t = 0.6f - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;

	if(t < 0.0f)
		{
			n1 = 0.0f;
		}
	else
		{
			t *= t;
			n1 = t * t * GradCoord4d(offset, i + i1, j + j1, k + k1, l + l1, x1, y1, z1, w1);
		}

	t = 0.6f - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;

	if(t < 0.0f)
		{
			n2 = 0.0f;
		}
	else
		{
			t *= t;
			n2 = t * t * GradCoord4d(offset, i + i2, j + j2, k + k2, l + l2, x2, y2, z2, w2);
		}

	t = 0.6f - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;

	if(t < 0.0f)
		{
			n3 = 0.0f;
		}
	else
		{
			t *= t;
			n3 = t * t * GradCoord4d(offset, i + i3, j + j3, k + k3, l + l3, x3, y3, z3, w3);
		}

	t = 0.6f - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;

	if(t < 0.0f)
		{
			n4 = 0.0f;
		}
	else
		{
			t *= t;
			n4 = t * t * GradCoord4d(offset, i + 1, j + 1, k + 1, l + 1, x4, y4, z4, w4);
		}

	return 27.0f * (n0 + n1 + n2 + n3 + n4);
}
}
