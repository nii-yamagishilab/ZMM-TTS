python3 txte2vec/synthesize.py --text "tts_test" --restore_step 200000 --mode quick_test --dataset MM6 --config MM6_XphoneBERT --input_textlist Dataset/MM6/test.txt --output_dir test_result/pred_vecfromxp --pretrained_model "paper_model/xp2vec/1200000.pth.tar" 
rm -rf test_result/vqtemp
mkdir  test_result/vqtemp
python test_scripts/prepare_for_vec2wav.py --Code_Dir test_result/pred_vecfromxp_conver/ --Test_text Dataset/MM6/test.txt --Test_temp test_result/vqtemp/ --Save_yaml test_result/vqtemp/pred.yaml
python vec2wav/infer.py -c  Config/vec2wav/vec2wav.yaml -m paper_model/vec2wav/model_1000000 -t test_result/vqtemp/pred.yaml -o test_result/zmm_tts2_xp
python3 vec2mel/synthesize.py --text "tts_test" --restore_step 195000 --mode quick_test --dataset MM6 --config MM6 --input_path Dataset/MM6/test.txt  --output_path test_result/pred_melfromvec --code_path test_result/pred_vecfromxp --pretrained_model "paper_model/vec2mel/1200000.pth.tar"
python Vocoder_HifiGAN_Model/inference_mel.py --input_wavs_dir test_result/pred_melfromvec/ --output_dir test_result/zmm_tts1_xp --checkpoint_file paper_model/HifiGAN/g_02500000

python3 txte2vec/synthesize.py --text "tts_test" --restore_step 200000 --mode quick_test --dataset MM6 --config MM6_Letters --input_textlist Dataset/MM6/test.txt --output_dir test_result/pred_vecfromxp --pretrained_model "paper_model/le2vec/1200000.pth.tar"
rm -rf test_result/vqtemp
mkdir  test_result/vqtemp
python test_scripts/prepare_for_vec2wav.py --Code_Dir test_result/pred_vecfromxp_conver/ --Test_text Dataset/MM6/test.txt --Test_temp test_result/vqtemp/ --Save_yaml test_result/vqtemp/pred.yaml
python vec2wav/infer.py -c  Config/vec2wav/vec2wav.yaml -m paper_model/vec2wav/model_1000000 -t test_result/vqtemp/pred.yaml -o test_result/zmm_tts2_le
python3 vec2mel/synthesize.py --text "tts_test" --restore_step 195000 --mode quick_test --dataset MM6 --config MM6 --input_path Dataset/MM6/test.txt  --output_path test_result/pred_melfromvec --code_path test_result/pred_vecfromxp --pretrained_model "paper_model/vec2mel/1200000.pth.tar"
python Vocoder_HifiGAN_Model/inference_mel.py --input_wavs_dir test_result/pred_melfromvec/ --output_dir test_result/zmm_tts1_le --checkpoint_file paper_model/HifiGAN/g_02500000
