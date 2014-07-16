#pragma once


namespace graphic
{

	class cCharacterAnalyzer : public cCharacter
	{
	public:
		cCharacterAnalyzer();
		virtual ~cCharacterAnalyzer();

		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;


	protected:


	private:
		
	};

}
