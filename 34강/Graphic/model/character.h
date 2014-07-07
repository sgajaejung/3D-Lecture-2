#pragma once


namespace graphic
{

	class cCharacter : public cModel
	{
	public:
		cCharacter();
		virtual ~cCharacter();

		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;


	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference
	};

}
