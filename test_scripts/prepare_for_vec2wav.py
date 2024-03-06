# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================




import os
import shutil
import argparse




def main():
 parser = argparse.ArgumentParser()
 parser.add_argument('--Code_Dir', type=str, required=True, help='Path to pretrained fairseq base model.')
 parser.add_argument('--Test_text', type=str, required=True, help='Path to pretrained fairseq base model.')
 parser.add_argument('--Test_temp', type=str, required=True, help='Path to pretrained fairseq base model.')
 parser.add_argument('--Save_yaml', type=str, required=True, help='Path to pretrained fairseq base model.')
 #parser.add_argument('--Test_fake', type=str, required=True, help='Path to pretrained fairseq base model.')
 #parser.add_argument('--Number', type=int, required=True, help='Path to pretrained fairseq base model.')
 args = parser.parse_args()


 data_dir = args.Code_Dir
 temp_dir = args.Test_temp

 with open (args.Save_yaml,'w') as f1, open(args.Test_text,'r') as f2:
    lines = f2.readlines()
    j = 0 
    for line in lines:
        if len(line.split('|')) == 6:
            language = line.split('|')[2]
            speaker_name = line.split('|')[-1].rstrip()
        else:
            speaker_name = line.split('|')[0].rstrip()
            language = line.split('|')[2]
        file_name = 'pred_mel_'+str(j)+'.npy'
        path1 = os.path.join(data_dir,file_name)
        base_name2 = str(j).zfill(4)
        
        f1.write(base_name2+'_generated:'+'\n')
        path2 = os.path.join(temp_dir,'Generate_'+language+'_'+str(j).zfill(4)+'_'+speaker_name+'.npy')
        shutil.copy(path1,path2)
        f1.write('  mel: '+path2+'\n')
        j = j + 1


if __name__ == '__main__':
    main()
