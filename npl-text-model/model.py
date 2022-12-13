from hashlib import new
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import LSTM
from keras.callbacks import ModelCheckpoint

def create_model(X_p, y_p, checkpoint_path):
    # define the LSTM model
    model = Sequential()
    model.add(LSTM(256, input_shape=(X_p.shape[1], X_p.shape[2])))
    model.add(Dropout(0.4))
    model.add(Dense(y_p.shape[1], activation='tanh'))
    model.add(Dropout(0.4))
    model.add(Dense(y_p.shape[1], activation='softmax'))
    model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
    # define the checkpoint
    # filepath="weights-improvement_-{epoch:02d}-{loss:.4f}.hdf5"
    checkpoint = ModelCheckpoint(checkpoint_path, monitor='loss', verbose=1, save_best_only=True, mode='min')
    callbacks_list = [checkpoint]

    return model, callbacks_list