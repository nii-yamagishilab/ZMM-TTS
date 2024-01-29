import math
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np


# Draw 2-D matrix ([time, dimension] -> [dimension, time])
def plot_matrix(matrix,
                path=None, aspect='equal', vmin=None, vmax=None,
                xlabel='', ylabel='', info=None,
                bbox_inches='tight', dpi=300, transparent=True,
                format='png'):
    plt.close('all')

    # Convert to figure
    if len(matrix.shape) == 3:
        height = int(math.ceil(np.sqrt(matrix.shape[0])))
        fig, ax = plt.subplots(height, height)
        for i in range(matrix.shape[0]):
            im = ax[i // height, i % height].imshow(
                matrix[i].T,
                aspect=aspect,
                origin='lower',
                interpolation='none',
                vmin=vmin, vmax=vmax)
    else:
        fig, ax = plt.subplots()
        im = ax.imshow(
            matrix.T,
            aspect=aspect,
            origin='lower',
            interpolation='none',
            vmin=vmin, vmax=vmax)

    # Set labels
    # fig.colorbar(im, ax=ax)
    if info is not None:
        xlabel += '\n\n' + info
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.tight_layout()

    # Save picture
    if path is None:
        fig.canvas.draw()
        data = np.fromstring(fig.canvas.tostring_rgb(), dtype=np.uint8, sep='')
        data = data.reshape(fig.canvas.get_width_height()[::-1] + (3,))
        return data
    else:
        plt.savefig(path,
                    bbox_inches=bbox_inches,
                    dpi=dpi,
                    transparent=transparent,
                    format=format)
    