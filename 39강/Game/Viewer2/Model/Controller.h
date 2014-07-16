#pragma once


class cController : public common::cObservable
							, public common::cSingleton<cController>							
{
public:
	cController();
	virtual ~cController();

	bool LoadFile( const string &fileName );
	graphic::cCharacter* GetCharacter();
	const string& GetCurrentMeshFileName();
	const string& GetCurrentAnimationFileName();

	void Render();
	void Update(const float elapseT);
	void SetAnimationPlay(const bool isPlay);
	bool IsAnimationPlay() const;
	void SetCurrentAnimationFrame(const int curFrame);


private:
	graphic::cCharacter *m_character;
	string m_currentMeshFileName;
	string m_currentAnimationFileName;
	bool m_isPlay;

};


inline graphic::cCharacter* cController::GetCharacter() { return m_character; }
inline const string& cController::GetCurrentMeshFileName() { return m_currentMeshFileName; }
inline const string& cController::GetCurrentAnimationFileName() { return m_currentAnimationFileName; }
inline void cController::SetAnimationPlay(const bool isPlay) { m_isPlay = isPlay; }
inline bool cController::IsAnimationPlay() const { return m_isPlay; }
