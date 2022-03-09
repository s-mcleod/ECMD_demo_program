from numpy import genfromtxt
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import csv
import sys
import glob


# Extracts the file name and directory from the path.
def get_file_name(path):
    filename = ""
    start_recording_filename = False
    for i in range(len(path)-1,0,-1):
        if (path[i] == '/'):
            return filename
        if (start_recording_filename):
            filename = path[i] + filename
        if (path[i] == '.'):
            start_recording_filename = True
    return filename

# Interprets the file name as a number
def get_file_number(file_location):
    filename = int(get_file_name(file_location))
    return filename

# Returns a list of all files in the directory and sorts them in numerical order.
def get_all_files_in_directory(path, extension):
    files = []
    for filepath in glob.iglob(path+'*'+extension):
        files.append(filepath)
    files.sort(key=get_file_number)
    return files


def main():

    path_to_events = sys.argv[1]
    path_to_warped_event_images = path_to_events + 'warped_event_images/'
    path_to_unwarped_event_images = path_to_events + 'unwarped_event_images/'

    warped_image_csv_files = get_all_files_in_directory(path_to_warped_event_images, ".csv")
    unwarped_image_csv_files = get_all_files_in_directory(path_to_unwarped_event_images, ".csv")

    start_file = 0
    end_file = len(warped_image_csv_files)
    if len(sys.argv) > 2:
        start_file_number = int(sys.argv[3])
        end_file_number = int(sys.argv[4])
        for i, csv_file in enumerate(warped_image_csv_files):
            if get_file_number(csv_file) == start_file_number:
                start_file = i
                end_file = i + (end_file_number-start_file_number)
                break
    #
    # padding = 120
    # width = 1280
    # height = 720

    for i in range(start_file, end_file):
        # Save warped image
        image_csv_data = genfromtxt(warped_image_csv_files[i], delimiter=',')
        im = Image.fromarray(image_csv_data)
        if im.mode != 'RGB':
            im = im.convert('RGB')
        # im = im.crop((padding,padding,width,height))
        im.save(warped_image_csv_files[i][:-4-len(str(get_file_number(warped_image_csv_files[i])))]+'images/'+str(get_file_number(warped_image_csv_files[i]))+'.jpg')

        # Save unwarped image
        image_csv_data = genfromtxt(unwarped_image_csv_files[i], delimiter=',')
        im = Image.fromarray(image_csv_data)
        if im.mode != 'RGB':
            im = im.convert('RGB')
        # im = im.crop((padding,padding,width,height))
        im.save(unwarped_image_csv_files[i][:-4-len(str(get_file_number(unwarped_image_csv_files[i])))]+'images/'+str(get_file_number(unwarped_image_csv_files[i]))+'.jpg')


if __name__ == "__main__":
    main()
