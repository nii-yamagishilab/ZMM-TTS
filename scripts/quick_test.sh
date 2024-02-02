python3 txte2vec/synthesize.py --text "tts_test" --restore_step 200000 --mode quick_test --dataset MM6 --config MM6_XphoneBERT --input_textlist Dataset/MM6/test.txt --output_dir test_result/pred_vecfromxp
rm -rf test_result/vqtemp
mkdir  test_result/vqtemp
python test_scripts/prepare_for_vec2wav.py --Code_Dir test_result/pred_vecfromxp_conver/ --Test_text Dataset/MM6/test.txt --Test_temp test_result/vqtemp/ --Save_yaml test_result/vqtemp/pred.yaml
python vec2wav/infer.py -c  Config/vec2wav/vec2wav.yaml -m Train_log/vec2wav/checkpoints/msmc_vq_gan/model_190000 -t test_result/vqtemp/pred.yaml -o test_result/zmm_tts2_xp
python3 vec2mel/synthesize.py --text "tts_test" --restore_step 195000 --mode quick_test --dataset MM6 --config MM6 --input_path Dataset/MM6/test.txt  --output_path test_result/pred_melfromvec --code_path test_result/pred_vecfromxp
python Vocoder_HifiGAN_Model/inference_mel.py --input_wavs_dir test_result/pred_melfromvec/ --output_dir test_result/zmm_tts1_xp --checkpoint_file Train_log/HifiGAN/g_00070000

python3 txte2vec/synthesize.py --text "tts_test" --restore_step 200000 --mode quick_test --dataset MM6 --config MM6_Letters --input_textlist Dataset/MM6/test.txt --output_dir test_result/pred_vecfromxp
rm -rf test_result/vqtemp
mkdir  test_result/vqtemp
python test_scripts/prepare_for_vec2wav.py --Code_Dir test_result/pred_vecfromxp_conver/ --Test_text Dataset/MM6/test.txt --Test_temp test_result/vqtemp/ --Save_yaml test_result/vqtemp/pred.yaml
python vec2wav/infer.py -c  Config/vec2wav/vec2wav.yaml -m Train_log/vec2wav/checkpoints/msmc_vq_gan/model_190000 -t test_result/vqtemp/pred.yaml -o test_result/zmm_tts2_le
python3 vec2mel/synthesize.py --text "tts_test" --restore_step 195000 --mode quick_test --dataset MM6 --config MM6 --input_path Dataset/MM6/test.txt  --output_path test_result/pred_melfromvec --code_path test_result/pred_vecfromxp
python Vocoder_HifiGAN_Model/inference_mel.py --input_wavs_dir test_result/pred_melfromvec/ --output_dir test_result/zmm_tts1_le --checkpoint_file Train_log/HifiGAN/g_00070000
