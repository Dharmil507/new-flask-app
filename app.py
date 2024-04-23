from flask import Flask, render_template, request, redirect, url_for, flash, send_from_directory, jsonify
from werkzeug.utils import secure_filename
import os
from OCC.Core.STEPControl import STEPControl_Reader
from OCC.Core.IFSelect import IFSelect_RetDone
from OCC.Core.BRepMesh import BRepMesh_IncrementalMesh
from OCC.Core.STEPControl import STEPControl_Writer
from OCC.Core.STEPControl import STEPControl_AsIs
from OCC.Core.StlAPI import StlAPI_Writer
import trimesh

app = Flask(__name__)
app.secret_key = 'your_secret_key'

UPLOAD_FOLDER = 'uploads'
ALLOWED_EXTENSIONS = {'step', 'stp', 'obj'}
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER


def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


def convert_to_stl(inputFile, outputFile):
    extension = inputFile.rsplit('.', 1)[1].lower()
    if extension in {'step', 'stp'}:
        return step_to_stl(inputFile, outputFile)
    elif extension == 'obj':
        return obj_to_stl(inputFile, outputFile)
    else:
        return False


def step_to_stl(inputFile, outputFile):
    step_reader = STEPControl_Reader()
    status = step_reader.ReadFile(inputFile)

    if status == IFSelect_RetDone:
        step_reader.TransferRoot(1)
        shape = step_reader.Shape(1)

        mesh = BRepMesh_IncrementalMesh(shape, 0.1)
        mesh.Perform()

        stl_writer = StlAPI_Writer()
        stl_writer.SetASCIIMode(True)
        stl_writer.Write(shape, outputFile)

        return True
    else:
        return False


def obj_to_stl(inputFile, outputFile):
    try:
        mesh = trimesh.load(inputFile)

        outputDir = os.path.dirname(outputFile)
        os.makedirs(outputDir, exist_ok=True)
        mesh.export(outputFile)

        return True
    except Exception as e:
        print(f"Error converting OBJ to STL: {e}")
        return False


@app.route('/', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        
        file = request.files['file']
        
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            input_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(input_path)
            
            stl_filename = filename.rsplit('.', 1)[0] + '.stl'
            stl_path = os.path.join(app.config['UPLOAD_FOLDER'], stl_filename)
            
            if convert_to_stl(input_path, stl_path):
                return redirect(url_for('stl_viewer'))
            else:
                flash('Error converting file to STL')
                return redirect(request.url)
    
    return render_template('upload.html')



@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)


@app.route('/get_uploaded_stl_files')
def get_uploaded_stl_files():
    uploaded_files = os.listdir(app.config['UPLOAD_FOLDER'])
    stl_files = [filename for filename in uploaded_files if filename.endswith('.stl')]
    return jsonify(stl_files)

@app.route('/stl_viewer')
def stl_viewer():
    uploaded_files = os.listdir(app.config['UPLOAD_FOLDER'])
    stl_files = [filename for filename in uploaded_files if filename.endswith('.stl')]
    if stl_files:
        # Assuming you want to display the latest uploaded file
        filename = stl_files[-1]
        return render_template('stl_viewer.html', filename=filename)
    else:
        return render_template('stl_viewer.html', filename=None)  # Handle case where no files are uploaded


if __name__ == '__main__':
    app.run(debug=True)