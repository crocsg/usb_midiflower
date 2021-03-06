import React from 'react';
import { Flex, Box} from 'rebass';
import { Label, Select } from '@rebass/forms'
//import Channel  from './Component/Channel'
import isDev from './devdetect';

export default class Channels extends React.Component {
    
  constructor(props) {
    super(props);
    this.state = {
        name:"Channels",
        chan:[],
        mul:["off", "1", "2", "4"],
        size:["8", "16", "32", "48", "64", "128"],
        ratio:["10", "25", "30", "50", "75", "100"]
    };
    
    
    this.handleChangeMul = this.handleChangeMul.bind(this);
    this.handleChangeSize = this.handleChangeSize.bind(this);
    this.handleChangeRatio = this.handleChangeRatio.bind(this);
    
    }

    handleChangeMul = event => {
      console.log ("changing mul");
      
      console.log(event.target);
      console.log(event.target.value);
      console.log("bpm value");
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let tmp = this.state.chan;
        this.state.chan[parseInt(event.target.id)].mul = event.target.value;
        this.setState (this.state.chan);
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&mul=' + event.target.value;
      
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        }
      
      
    };

    handleChangeSize = event => {
      console.log ("changing size");
      this.setState ({bpm:event.target.value});
      console.log(event.target);
      console.log(event.target.value);
      console.log("bpm value");
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let tmp = this.state.chan;
        this.state.chan[parseInt(event.target.id)].size = event.target.value;
        this.setState (this.state.chan);
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&size=' + event.target.value;
      
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        }
      
      
    };
    handleChangeRatio = event => {
      console.log ("changing size");
      this.setState ({bpm:event.target.value});
      console.log(event.target);
      console.log(event.target.value);
      console.log("bpm value");
      
      if (parseInt(event.target.id) < this.state.chan.length)
      {
        let tmp = this.state.chan;
        this.state.chan[parseInt(event.target.id)].size = event.target.value;
        this.setState (this.state.chan);
        let url = window.url_prefix + '/config/setdata/chan?channel='+ event.target.id + '&ratio=' + event.target.value;
      
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
                  value={channel.mul}
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
                  value={channel.size}
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
                  value={channel.ratio}
                  onChange={this.handleChangeRatio}
                  >
                      {Object.entries(this.state.ratio).map(([ key, value ]) => (
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
  