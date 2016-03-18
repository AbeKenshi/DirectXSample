#ifndef _ENTITY_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _ENTITY_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"
#include "audio.h"

namespace entityNS
{
	enum COLLISION_TYPE {
		NONE,
		CIRCLE,
		BOX,
		ROTATED_BOX
	};
	const float GRAVITY = 6.67428e-11f;
}

//=============================================================================
//=============================================================================
class Entity : public Image
{
protected:
	// Entityプロパティ
	entityNS::COLLISION_TYPE collisionType;		// 衝突の種類
	VECTOR2 center;								// エンティティの中心
	float radius;								// 円の衝突判定に使用する、円の半径
	VECTOR2 distSquared;						// 円の衝突判定で計算されるベクトル
	float sumRadiiSquared;
	RECT edge;									// BOXまたはROTATED_BOXの衝突判定が使用する、矩形領域
	VECTOR2 corners[4];							// 境界ボックスの頂点
	VECTOR2 edge01, edge03;						// 投影線
	float   edge01Min, edge01Max, edge03Min, edge03Max; // 投影の最小値と最大値
	VECTOR2 velocity;							// 現在のエンティティの速度を含むベクトル
	VECTOR2 deltaV;								// 
	float mass;									// エンティティの質量
	float health;								// エンティティの体力（0から100）
	float rr;									// Radius squared variable
	float force;								// Force of gravity
	float gravity;								// 
	Input *input;								// 入力システムへのポインタ
	Audio *audio;								// オーディオシステムへのポインタ
	HRESULT hr;									// 標準の戻り値
	bool active;								// エンティティの現在の状態。アクティブなエンティティのみが衝突を起こします。
	bool rotatedBoxReady;						// ROTATED_BOXの衝突判定のとき、true

	// 円の衝突判定メソッド
	// collision()によって呼び出される、デフォルトの衝突判定メソッド
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);

	// 軸並行境界ボックスの衝突判定メソッド
	// collision()によって呼び出される
	// 実行後：衝突している場合はtrue、していない場合はfalseを返す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);

	// 回転するボックスの衝突判定メソッド
	// collision()によって呼び出される
	// 実行後：衝突している場合はtrue、していない場合はfalseを返す
	// 衝突している場合は、collisionVectorを設定
	// 分離軸テストを使用して衝突を判定
	// 分離軸テスト：
	// 2つのボックスは、特定の直線への射影が重なっていなければ衝突していない
	virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);

	// 回転するボックスと円の衝突判定メソッド
	// collision()によって呼び出される
	// ボックスの辺と円の半径で分離軸テストを使用
	// 円の中心が衝突ボックスの各辺を伸ばした直線の外側
	// （ボロノイ領域と呼ばれる）にある場合、ボックスの最も近い頂点と
	// 衝突がないかを距離のチェックで判定します。
	// 最も近い頂点は、重なりのテストから判断できます。
	//
	//   Voronoi0 |   | Voronoi1
	//         ---0---1---
	//            |   |
	//         ---3---2---
	//   Voronoi3 |   | Voronoi2
	//
	// 実行前：このエンティティは必ずボックス、相手のエンティティ（ent）は必ず円
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
	
	// 回転するボックスの頂点、投影線、投影の最小値と最大値を計算
	// 0---1  頂点の番号
	// |   |
	// 3---2
	void computeRotatedBox();

	// 相手のボックスを、このエンティティのedge01およびedge03に投影
	// collideRotateBox()によって呼び出される
	// 実行後：投影が重なっている場合はtrue、それ以外の場合はfalseを戻す
	bool projectionsOverlap(Entity &ent);

	// ボックスの頂点が円と衝突していないかを、距離のチェックを使用して判定
	// collideRotateBoxCircle()によって呼び出される
	// 実行後：衝突指定場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

public:
	// コンストラクタ
	Entity();

	// デストラクタ
	virtual ~Entity() {}

	// エンティティの画面上での中心座標を戻す
	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	// 円の衝突判定に使用する円の半径を戻す
	virtual float getRadius() const
	{
		return radius;
	}

	// BOXまたはROTATED_BOXの衝突判定が使用する、矩形領域を戻す
	virtual const RECT& getEdge() const
	{
		return edge;
	}

	// Return corner c of ROTATED_BOX
	virtual const VECTOR2* getCorner(UINT c) const
	{
		if (c >= 4)
			c = 0;
		return &corners[c];
	}

	// Return velocity vector.
	virtual const VECTOR2 getVelocity() const { return velocity; }

	// Return active.
	virtual bool  getActive()         const { return active; }

	// Return mass.
	virtual float getMass()           const { return mass; }

	// Return gravitational constant.
	virtual float getGravity()        const { return gravity; }

	// Return health;
	virtual float getHealth()         const { return health; }

	// Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
	virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }

	////////////////////////////////////////
	//           Set functions            //
	////////////////////////////////////////

	// Set velocity.
	virtual void  setVelocity(VECTOR2 v) { velocity = v; }

	// Set delta velocity. Added to velocity in update().
	virtual void  setDeltaV(VECTOR2 dv) { deltaV = dv; }

	// Set active.
	virtual void  setActive(bool a) { active = a; }

	// Set health.
	virtual void setHealth(float h) { health = h; }

	// Set mass.
	virtual void  setMass(float m) { mass = m; }

	// Set gravitational constant. Default is 6.67428e-11
	virtual void  setGravity(float g) { gravity = g; }

	// Set radius of collision circle.
	virtual void setCollisionRadius(float r) { radius = r; }

	////////////////////////////////////////
	//         Other functions            //
	////////////////////////////////////////

	// Update Entity.
	// typically called once per frame
	// frameTime is used to regulate the speed of movement and animation
	virtual void update(float frameTime);

	// Entityを初期化
	// 実行前：*gamePtr = Gameオブジェクトへのポインタ
	//		   width = Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
	//		   height = Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
	//		   ncols = テクスチャ内の列数（1からnまで）（0は1と同じ）
	//		   *textureM = TextureManagerオブジェクトへのポインタ
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);

	// エンティティをアクティブ化
	virtual void activate();

	// Empty ai function to allow Entity objects to be instantiated.
	virtual void ai(float frameTime, Entity &ent);

	// Is this entity outside the specified rectangle?
	virtual bool outsideRect(RECT rect);

	// Does this entity collide with ent?
	virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

	// Damage this Entity with weapon.
	virtual void damage(int weapon);

	// Entity bounces after collision with other Entity
	void bounce(VECTOR2 &collisionVector, Entity &ent);

	// Adds the gravitational force to the velocity vector of this entity
	void gravityForce(Entity *other, float frameTime);

};

#endif