#pragma once
#include"seedPoint.h"
#define SORT_SEED_RATE   0.5
//�����ӵ�����������������ȴ������λ�ÿ������������ĵ����ӵ�
void sortSeed(SeedSet& seedMergeS, const cv::Mat& outline);
//�������ӵ�������������ĵ����λ��
double calculateRelativePosition(Seed& s, const cv::Mat& outline);
// �жϾ����Ƿ�Ϊ�յĺ���  
bool isRectEmpty(const cv::Rect& rect);
//��ʾ���������ӵ�
cv::Mat drawSortSeed(const cv::Mat& outline, const SeedSet& seedS, cv::Scalar s);
