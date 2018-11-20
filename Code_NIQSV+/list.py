# -*- coding: utf-8 -*-
import xlrd
import os

workbook = xlrd.open_workbook('D:\Dataset\IRCCyN_IVC_DIBR_Images\IRCCyN_IVC_DIBR_Images_Database_ACR_Score.xlsx')
sheet_names = workbook.sheet_names()

print(sheet_names)

sheet = workbook.sheet_by_name(sheet_names[1])

f = open('score.txt', 'w')

for item in sheet.col_values(46):
    if item is not None:
        f.write(str(item))
        f.write('\n')

f.close()