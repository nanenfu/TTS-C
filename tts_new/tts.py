import onnxruntime as ort
import numpy as np
import soundfile as sf 
from get_ph import TextPreprocessor 

np.random.seed(42)
# First function: Perform inference on the ONNX encoder and decoder
def run_t2s_onnx_model(onnx_model_paths, ref_seq, text_seq, ref_bert, text_bert, ssl_content, early_stop_num):
    # Load ONNX models
    onnx_encoder_path, onnx_fsdec_path, onnx_sdec_path = onnx_model_paths
    print(f"Loading ONNX models from {onnx_encoder_path}, {onnx_fsdec_path}, {onnx_sdec_path}")

    # Load ONNX sessions
    session_options = ort.SessionOptions()
    ort.set_seed(65)
    encoder_session = ort.InferenceSession(onnx_encoder_path)
    fsdec_session = ort.InferenceSession(onnx_fsdec_path)
    sdec_session = ort.InferenceSession(onnx_sdec_path)

    # Get input and output names
    encoder_input_names = [inp.name for inp in encoder_session.get_inputs()]
    encoder_output_names = [out.name for out in encoder_session.get_outputs()]

    fsdec_input_names = [inp.name for inp in fsdec_session.get_inputs()]
    fsdec_output_names = [out.name for out in fsdec_session.get_outputs()]

    sdec_input_names = [inp.name for inp in sdec_session.get_inputs()]
    sdec_output_names = [out.name for out in sdec_session.get_outputs()]

    # Prepare input data (using numpy arrays)
    ref_seq_numpy = ref_seq
    text_seq_numpy = text_seq
    ref_bert_numpy = ref_bert
    text_bert_numpy = text_bert
    ssl_content_numpy = ssl_content

    # Perform inference on ONNX encoder
    print("Running ONNX Encoder...")
    encoder_inputs = {
        encoder_input_names[0]: ref_seq_numpy,
        encoder_input_names[1]: text_seq_numpy,
        encoder_input_names[2]: ref_bert_numpy,
        encoder_input_names[3]: text_bert_numpy,
        encoder_input_names[4]: ssl_content_numpy
    }

    # Print the types and shapes of the inputs
    print(f"ref_seq type: {ref_seq_numpy.dtype}, shape: {ref_seq_numpy.shape}")
    print(f"text_seq type: {text_seq_numpy.dtype}, shape: {text_seq_numpy.shape}")
    print(f"ref_bert type: {ref_bert_numpy.dtype}, shape: {ref_bert_numpy.shape}")
    print(f"text_bert type: {text_bert_numpy.dtype}, shape: {text_bert_numpy.shape}")
    print(f"ssl_content type: {ssl_content_numpy.dtype}, shape: {ssl_content_numpy.shape}")


    encoder_outputs = encoder_session.run(encoder_output_names, encoder_inputs)
    x, prompts = encoder_outputs
    print(f"Encoder output shapes: x = {x.shape}, prompts = {prompts.shape}")

    # Perform inference on the first-stage decoder
    print("Running ONNX First Stage Decoder...")
    fsdec_inputs = {
        fsdec_input_names[0]: x,
        fsdec_input_names[1]: prompts
    }

    fsdec_outputs = fsdec_session.run(fsdec_output_names, fsdec_inputs)
    y, k, v, y_emb, x_example = fsdec_outputs
    print(f"First Stage Decoder output shapes: y = {y.shape}, k = {k.shape}, v = {v.shape}, y_emb = {y_emb.shape}, x_example = {x_example.shape}")

    # Perform inference on the second-stage decoder, with early stop logic
    print("Running ONNX Second Stage Decoder with early stopping...")
    stop = False
    prefix_len = prompts.shape[1]
    for idx in range(1, 1500):
        sdec_inputs = {
            sdec_input_names[0]: y,
            sdec_input_names[1]: k,
            sdec_input_names[2]: v,
            sdec_input_names[3]: y_emb,
            sdec_input_names[4]: x_example
        }

        sdec_outputs = sdec_session.run(sdec_output_names, sdec_inputs)
        y, k, v, y_emb, logits, samples = sdec_outputs

        # Early stop logic
        if early_stop_num != -1 and (y.shape[1] - prefix_len) > early_stop_num:
            stop = True
            print(f"early_stop_num != -1 and (y.shape[1] - prefix_len) > early_stop_num")
        if np.argmax(logits, axis=-1)[0] == 1024 or samples[0, 0] == 1024: 
            stop = True
        if stop:
            print(f"Stopping at iteration {idx} due to early stop condition.")
            break

    # Slice the last few time steps of y (based on idx) and save
    y[0, -1] = 0
    y_sliced = y[:, -idx:]  # Perform slicing
    y_unsqueezed = np.expand_dims(y_sliced, axis=0)  
    print(f"Final y shape after stopping and slicing: {y_unsqueezed.shape}")
    
    return y_unsqueezed

# Second function: Perform inference on VITS ONNX model to generate audio
def run_vits_onnx_model(onnx_model_path, text_seq, pred_semantic):
    # Load ONNX model
    print(f"Loading ONNX model from {onnx_model_path}")
    session = ort.InferenceSession(onnx_model_path)

    # Check model inputs and outputs
    input_names = [inp.name for inp in session.get_inputs()]
    output_names = [out.name for out in session.get_outputs()]
    print(f"Input names: {input_names}")
    print(f"Output names: {output_names}")

    # Prepare input
    text_seq_numpy = text_seq  
    pred_semantic_numpy = pred_semantic 

    inputs = {
        input_names[0]: text_seq_numpy,  # text_seq
        input_names[1]: pred_semantic_numpy,  # pred_semantic
    }

    # Perform inference
    print("Running inference on ONNX model...")
    outputs = session.run(output_names, inputs)

    # Print output
    audio_output = outputs[0]
    print(f"Generated audio shape: {audio_output.shape}")
    return audio_output


if __name__ == "__main__":
    # Step 1: Run T2S model to generate y
    project_name = "test3"
    onnx_encoder_path = f"onnx/{project_name}/{project_name}_t2s_encoder.onnx"
    onnx_fsdec_path = f"onnx/{project_name}/{project_name}_t2s_fsdec.onnx"
    onnx_sdec_path = f"onnx/{project_name}/{project_name}_t2s_sdec.onnx"
    onnx_model_path = f"onnx/{project_name}/{project_name}_vits.onnx"

    # Step 1: Use the TextPreprocessor class to get text_seq
    text = "just the two of us, we can make it if we try"
    text_preprocessor = TextPreprocessor()
    processed_text = text_preprocessor.preprocess(text, "en", "cut4")
    # Convert the processed result into numpy format for text_seq
    all_phones = [phone for result in processed_text for phone in result['phones']]
    text_seq = np.array([all_phones])
    print(f"text_seq = {text_seq}")
    
    ref_seq = np.array([[10, 64, 26, 75, 42, 1, 64, 68, 1, 64, 68, 1, 55, 80, 75, 68, 61, 42, 1, 51, 58, 68, 64, 62, 57, 75, 63, 68, 61, 75, 91, 35, 64, 51, 58, 26, 74, 55, 65, 61, 75, 3, 26, 84, 74, 54, 65, 27, 12, 63, 7, 64, 12, 62, 17, 50, 1, 91, 13, 80, 22, 62, 26, 88, 55, 93, 22, 62, 60, 13, 75, 80, 80, 16, 61, 80, 88, 10, 64, 7, 26, 57, 12, 64, 75, 63, 35, 63, 24, 38, 3]])
    # text_seq = np.array([[60, 13, 75, 80, 27, 12, 80, 88, 13, 90, 13, 75, 1, 91, 58, 61, 10, 64, 63, 42, 61, 55, 80, 55, 49, 91, 58, 80, 74, 22, 3]])
    ref_bert = np.zeros((ref_seq.shape[1], 1024), dtype=np.float32)
    text_bert = np.zeros((text_seq.shape[1], 1024), dtype=np.float32)
    ssl_content = np.load('./ssl_content.npy')  

    # early_stop_num logic
    early_stop_num = 2700

    # Run the T2S model
    y = run_t2s_onnx_model(
        (onnx_encoder_path, onnx_fsdec_path, onnx_sdec_path),
        ref_seq, text_seq, ref_bert, text_bert, ssl_content, early_stop_num
    )

    # Step 2: Run VITS model to generate audio
    pred_semantic = y 
    audio_output = run_vits_onnx_model(onnx_model_path, text_seq, pred_semantic)

    # Save the generated audio
    audio_output = np.squeeze(audio_output)  
    sf.write("output.wav", audio_output, 32000)  
    print("Audio saved as 'output.wav'")
