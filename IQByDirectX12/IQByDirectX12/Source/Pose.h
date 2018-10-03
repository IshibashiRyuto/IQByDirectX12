/*
	Pose.h
	�{�[�����𓝊����A�p�������i�[����
	@author Ishibashi Ryuto
	@date
	2018/09/12	���ō쐬
	2018/09/14	�R���X�g���N�^���B���Ashared_ptr��Ԃ�Create���\�b�h���`
	2018/10/01	�{�[���̖��O�ƃC���f�b�N�X��Ή��t���A���O����C���f�b�N�X���擾�ł���悤�ɂ���
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <list>
#include <vector>
#include <map>

/* ����w�b�_�C���N���[�h */
#include "Math/Math.h"

/* �N���X�g�p�錾 */
class Bone;

typedef std::map< int, std::list<int>> ChildBoneList;
typedef std::map< std::wstring, int> BoneNameMap;

class Pose
{
public:
	/// �f�X�g���N�^
	~Pose();
	
	/// @fn Create
	/// �p�����N���X���쐬����
	/// @param[in] boneCount	: �p�������{�[���̐�
	static std::shared_ptr<Pose> Create(int boneCount);

	/// @fn Create
	/// �p�����N���X���쐬����
	/// @param[in] boneCount	: �p�������{�[���̐�
	static std::shared_ptr<Pose> Create(std::shared_ptr<Pose> defaultPose);

	/// @fn CalcPose
	/// �p���v�Z���s��
	void CalcPose();

	/// @fn GetBones
	/// �{�[���z����擾����
	/// @retval �{�[���z��̎Q��
	const std::vector<std::shared_ptr<Bone>> & GetBones();

	/// @fn IsFindBoneName
	/// �w�肵�����O�̃{�[�������݂��邩�`�F�b�N����
	/// @param[in] �{�[����
	/// @retval �{�[�������݂���	: true
	/// @retval �{�[�������݂��Ȃ�	: false
	bool IsFindBoneName(const std::wstring & boneName);

	/// @fn GetBoneIndex
	/// �{�[��������{�[���̃C���f�b�N�X���擾����
	/// @param[in]	boneName	: �{�[����
	///	@retval		�{�[���̃C���f�b�N�X
	/// @note		�w�肵���{�[�������݂��邩��
	///				IsFindBoneName�Ŏ��O�Ƀ`�F�b�N����K�v����
	int GetBoneIndex(const std::wstring& boneName);

	/// @fn SetBoneData
	/// �{�[������ǉ�����
	/// @param[in] boneName: �{�[����
	/// @param[in] bone	:	 �{�[�����
	/// @param[in] boneIndex : ���g�̃C���f�b�N�X
	/// @param[in] parentBoneIndex : �e�̃C���f�b�N�X
	/// @note ���ׂĂ̐e�m�[�h��-1�Ƃ��A�e�m�[�h�ɐڑ�����Ă��Ȃ��{�[����
	/// �p���v�Z�ΏۊO�ɂȂ�
	void SetBoneData(const std::wstring& boneName, const std::shared_ptr<Bone> bone, int boneIndex, int parentBoneIndex = -1);

	/// @fn Lerp
	/// �p�������Ԃ���
	/// @param[in] prePose : time = 0.0 �̏ꍇ�̎p��
	/// @param[in] postPose : time = 1.0 �̏ꍇ�̎p��
	/// @param[in] time	: ��ԌW��( 0.0 <= time <= 1.0 �ɃN�����v����� )
	/// @retval �p���s��v�Z�O�̎p��
	static std::shared_ptr<Pose> Lerp(const std::shared_ptr<Pose> prePose, const std::shared_ptr<Pose> postPose, float time);

private:
	/* �萔��` */
	const int ROOT_BONE_INDEX = -1;

	/* �����o�ϐ��錾 */
	std::vector<std::shared_ptr<Bone>> mBones;
	std::shared_ptr< ChildBoneList > mChildBoneList;
	std::shared_ptr< BoneNameMap > mBoneNameMap;
	
	/* �v���C�x�[�g���\�b�h��` */

	/// �R���X�g���N�^
	Pose();

	/// �R���X�g���N�^
	/// @param[in] boneCount : �p���̃{�[����
	Pose(int boneCount);

	/// �R�s�[�R���X�g���N�^
	Pose(const Pose& src);

	/// @fn CalcBoneMatrix
	/// �{�[���s����v�Z����
	/// @param[in] matrix : �{�[���ό`�s��
	/// @param[in] childList : ���g�̎q�{�[��
	void CalcBoneMatrix(const Math::Matrix4x4& mat, const std::list<int> & childBoneList);
};

