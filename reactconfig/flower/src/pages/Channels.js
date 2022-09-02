import React from 'react';
import { Flex, Box} from 'rebass';
import { Label, Select } from '@rebass/forms'
import Channel  from './Component/Channel'
import isDev from './devdetect';

export default class Channels extends React.Component {
    
  constructor(props) {
    super(props);
    this.state = {
        name:"Channels",
        chan:[],
        mul:["off", "1", "2", "4"],
        size:["8", "16", "32", "48", "64", "128"],
        ratio:["10", "25", "30", "50", "75", "100"],
        volumes:[80, 81, 82, 83, 84, 85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100],
        channels:[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
        volume:100
    };
    
    
    this.handleChangeMul = this.handleChangeMul.bind(this);
    this.handleChangeSize = this.handleChangeSize.bind(this);
    this.handleChangeRatio = this.handleChangeRatio.bind(this);
    this.handleChangeVolume = this.handleChangeVolume.bind(this);
    this.handleChangeChannel = this.handleChangeChannel.bind(this);

    }

    handleChangeMul = event => {
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let chan = this.state.chan;
        chan[parseInt(event.target.id)].ml = event.target.value;
        this.setState ({chan:chan});
        
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&mul=' + event.target.value;
      
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        }
      
      
    };

    handleChangeSize = event => {
      
      
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let chan = this.state.chan;
        chan[parseInt(event.target.id)].n = event.target.value;
        this.setState ({chan:chan});

        
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&size=' + event.target.value;
      
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        }
      
      
    };
    handleChangeRatio = event => {
      
      
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let chan = this.state.chan;
        chan[parseInt(event.target.id)].r = event.target.value;
        this.setState ({chan:chan});
        
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&ratio=' + event.target.value;
      
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        }
      
      
    };
    handleChangeVolume = event => {
        
        
        console.log(event.target);
        console.log(event.target.value);
        console.log("bpm value");
        
        if (parseInt(event.target.id) < this.state.chan.length)
        {
            let chan = this.state.chan;
            chan[parseInt(event.target.id)].v = event.target.value;
            this.setState ({chan:chan});

          
          let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&vol=' + event.target.value;
        
          fetch (url).finally(
              () => {
                  this.loaddata()
                  }
              )    
          }
        
        
      };
      handleChangeChannel = event => {
        
        this.setState ({volume:event.target.value});
        console.log(event.target);
        console.log(event.target.value);
        console.log("channel value");
        
        if (parseInt(event.target.id) < this.state.chan.length)
        {
            let chan = this.state.chan;
            chan[parseInt(event.target.id)].c = event.target.value;
            this.setState ({chan:chan});

          let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&mch=' + event.target.value;
        
          fetch (url).finally(
              () => {
                  this.loaddata()
                  }
              )    
          }
        
        
      };  
    componentDidMount ()
    {
      this.loaddata();
    }

  loaddata ()
  {
    let url = window.url_prefix + '/config/param/channeldev.json';
    if (isDev()) {
        console.log('Development');
        
    } else {
        console.log('Production');
        url = window.url_prefix + '/config/param/channel.json';
    }

    console.log ("fetching data");
    
    console.log (url);
    fetch (url)
        .then (res => res.json())
        .catch (() => {
            this.setState ({fetchmsg:"error loading json"})
            })
        .then (
            (result) => {
                if (result)
                {
                    console.log (result);
                    this.setState ({chan:result.channels})
                    //this.setState ({scale:result.scale})
                    //this.setState ({notes:result.notes})
                    //this.setState ({note:result.note})
                    //this.setState ({bpms:result.bpms})
                    //this.setState ({bpm:result.bpm})
                }

            }
        )    
    }

  render () {
    console.log ("render", this.state);
      return (
        <Flex>
        <Box p={3} color='white'  bg='#ff3b25aFF' width={'20%'} ></Box>
        <Box p={3} color='white'  bg='#ff3b25aFF' width={'60%'} >
        <Box p={3} color='white' bg='#293b25b0' width={'100%'} > 
        <h1>Channels Parameters</h1>
        
          {this.state.chan.map((channel,index) =>   
              <>
              <h2>Channel {index}</h2>
              
              <Label htmlFor='chan{index}'>BPM multiplier </Label>
              <Select
                  alignSelf={'center'}
                  
                  width={'100%'}
                  color='white' bg='#293b25'
                  id={index}
                  name={'chan' + index}
                  value={channel.ml}
                  onChange={this.handleChangeMul}
                  >
                      {Object.entries(this.state.mul).map(([ key, value ]) => (
                      <option
                          key={key}>
                          {value}
                      </option>
                  ))}
              </Select>
              <Label htmlFor='chan{index}'>Size (Nb notes) </Label>
              <Select
                  alignSelf={'center'}
                  
                  width={'100%'}
                  color='white' bg='#293b25'
                  id={index}
                  name={'chan' + index}
                  value={channel.n}
                  onChange={this.handleChangeSize}
                  >
                      {Object.entries(this.state.size).map(([ key, value ]) => (
                      <option
                          key={key}>
                          {value}
                      </option>
                  ))}
              </Select>
              <Label htmlFor='chan{index}'>Fill Ratio (max notes) </Label>
              <Select
                  alignSelf={'center'}
                  
                  width={'100%'}
                  color='white' bg='#293b25'
                  id={index}
                  name={'chan' + index}
                  value={channel.r}
                  onChange={this.handleChangeRatio}
                  >
                      {Object.entries(this.state.ratio).map(([ key, value ]) => (
                      <option
                          key={key}>
                          {value}
                      </option>
                  ))}
              </Select>
              <Label htmlFor='chan{index}'>Relative Velocity (%)</Label>
              <Select
                  alignSelf={'center'}
                  
                  width={'100%'}
                  color='white' bg='#293b25'
                  id={index}
                  name={'chan' + index}
                  value={channel.v}
                  onChange={this.handleChangeVolume}
                  >
                      {Object.entries(this.state.volumes).map(([ key, value ]) => (
                      <option
                          key={key}>
                          {value}
                      </option>
                  ))}
              </Select>
              <Label htmlFor='chan{index}'>Midi channel</Label>
              <Select
                  alignSelf={'center'}
                  
                  width={'100%'}
                  color='white' bg='#293b25'
                  id={index}
                  name={'chan' + index}
                  value={channel.c}
                  onChange={this.handleChangeChannel}
                  >
                      {Object.entries(this.state.channels).map(([ key, value ]) => (
                      <option
                          key={key}>
                          {value}
                      </option>
                  ))}
              </Select>
              </>           
              
          )}

        </Box>
        
        </Box>
        <Box p={3} color='white'  bg='#ff3b25aFF' width={'20%'} ></Box>
        
        </Flex>
      );
    }
};
  