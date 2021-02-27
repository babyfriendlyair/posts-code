import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sn

df = pd.read_csv('all_sensors_multi_sample_21feb_19_45.csv', names=[
    'avg30Mq131Black',
    'avgMq131Black',
    'mq131Black',
    'avg30Mq131Silver',
    'avgMq131Silver',
    'mq131Silver',
    'avg30Ze25',
    'avgZe25',
    'ze25',
    'humidity',
    'temp']);

df = df.reset_index()

berlinData = pd.read_csv('ber_mc174_20210221-20210225.csv',
                         skiprows=3,
                         sep=';')

# Remove 15min worth of data to get start at 20:00
hourlyData = df.iloc[900:]

hourlyData = df.groupby(df.index // 3600).mean()

def addTime(row):
    row['time'] = (20 + row['index'] // 3600) % 24
    return row

hourlyData = hourlyData.apply(addTime, axis=1)

sensorAndBerlinData = pd.concat([hourlyData, berlinData], axis=1)

toPlot = [
    [('mq131Black', 0.1), ('avg30Mq131Black', 0.7), ('avgMq131Black', 0.9)],
    [('mq131Silver', 0.1), ('avg30Mq131Silver', 0.7), ('avgMq131Silver', 0.9)],
    [('ze25', 0.1), ('avg30Ze25', 0.7), ('avgZe25', 0.9)],
    [('humidity', 1)],
    [('temp', 1)]
]

for sensor in toPlot:
    fig, axs = plt.subplots(2)
    fig.set_size_inches(8, 6)
    fig.suptitle(sensor[0][0])
    axs[0].set_title('By Second')
    axs[1].set_title('Hourly Average')
    plt.sca(axs[1])
    plt.xticks(hourlyData['index'], hourlyData['time'].apply(round), fontsize=5, rotation=60)
    plt.grid()
    
    for series in sensor:
        axs[0].plot('index', series[0], data=df, alpha=series[1], label=series[0])
        axs[1].plot('index', series[0], data=hourlyData, alpha=series[1], label=series[0])

    axs[0].legend()
    axs[1].legend()
    plt.tight_layout()
    plt.savefig(sensor[0][0] + '.png', dpi=300)

plt.figure()
corrMatrix = sensorAndBerlinData[['avgMq131Black', 'avgMq131Silver', 'avgZe25', 'temp', 'humidity', 'ber_no', 'ber_no2', 'ber_nox', 'ber_o3']].corr()
sn.heatmap(corrMatrix, annot=True)

plt.tight_layout()
plt.savefig('corrMatrix_BER.png', dpi=300)
