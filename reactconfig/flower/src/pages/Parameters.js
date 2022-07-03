import React from 'react';
import { Button, Box} from 'rebass';
import { Label, Select } from '@rebass/forms'
import isDev from './devdetect';

export default class Parameters extends React.Component {
    
    constructor(props) {
        super(props);
        this.state = {
            name:"Parameters",
            scales:[],
            scale:"?",
            notes:[],
            rootnote:"?",
            bpms:[],
            bpm:[]
        };
        
        this.handleChangeScale = this.handleChangeScale.bind(this);
        this.handleChangeNote = this.handleChangeNote.bind(this);
        this.handleChangeBPM = this.handleChangeBPM.bind(this);
        
    }
    componentDidMount ()
    {
        this.loaddata();
    }



    loaddata ()
    {
        let url = window.url_prefix + '/config/param/paramdev.json';
        if (isDev()) {
            console.log('Development');
            
        } else {
            console.log('Production');
            url = window.url_prefix + '/config/param/param.json';
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
                        this.setState ({scales:result.scales})
                        this.setState ({scale:result.scale})
                        this.setState ({notes:result.notes})
                        this.setState ({note:result.note})
                        this.setState ({bpms:result.bpms})
                        this.setState ({bpm:result.bpm})
                    }

                }
            )    
    }
    
    handleChangeScale = event => {
        console.log ("changing scale");
        this.setState ({scale:event.target.value});
        console.log(event.target.value);
        console.log("scale value");
        let scale = 0;
        for (let i = 0; i < this.state.scales.length; i++)
        {
            if (this.state.scales[i] === event.target.value)
            {   
                scale = i;
                break;
            }
        }
        let url = window.url_prefix + '/config/setdata/data?scale='+ scale.toString();
        fetch (url).finally(
                () => {
                    this.loaddata()
                    }
                );    
      };
    
    handleChangeNote = event => {
        console.log ("changing note");
        this.setState ({note:event.target.value});
        console.log(event.target.value);
        console.log("note value");
        let root = 0;
        for (let i = 0; i < this.state.notes.length; i++)
        {
            if (this.state.notes[i] === event.target.value)
            {   
                root = i;
                break;
            }
        }
        let url = window.url_prefix + '/config/setdata/data?root='+ root.toString();
        fetch (url).finally(
                () => {
                    this.loaddata()
                    }
                );    
    };
    
    
    handleChangeBPM = event => {
        console.log ("changing bpm");
        this.setState ({bpm:event.target.value});
        console.log(event.target.value);
        console.log("bpm value");

        let url = window.url_prefix + '/config/setdata/data?bpm='+ event.target.value.toString();
        fetch (url).finally(
            () => {
                this.loaddata()
                }
            )    
        };

    render () {
        //defaultValue={this.state.note}
        console.log ("scale rendered", this.state.scale);
        return (
        <Box color='white' bg='#293b25a90' width={'100%'} >

            
            <Label htmlFor='scale'>Scale</Label>
            <Select
                alignSelf={'center'}
                
                width={'50%'}
                color='white' bg='#293b25'
                id='idscale'
                name='Scale'
                value={this.state.scale}
                onChange={this.handleChangeScale}
                >
                    {Object.entries(this.state.scales).map(([ key, scale ]) => (
                    <option
                        key={key}>
                        {scale}
                    </option>
                ))}
            </Select>
            

            <Box  color='white' bg='#ff3b2590' width={'50%'} >
            <Label htmlFor='note'>Root note</Label>
            <Select
            
                width={'50%'}
                color='white' bg='#293b25'
                id='idnote'
                name='Note'
                value={this.state.note}
                onChange={this.handleChangeNote}
                >
                    {Object.entries(this.state.notes).map(([ key, note ]) => (
                    <option
                        key={key}>
                        {note}
                    </option>
                ))}
            </Select>
            </Box>
            <Label htmlFor='bpm'>BPM</Label>
            <Select
            alignSelf={'center'}
                width={'50%'}
                color='white' bg='#293b25'
                id='idbpm'
                name='bpm'
                value={this.state.bpm}
                onChange={this.handleChangeBPM}
                >
                    {Object.entries(this.state.bpms).map(([ key, bpmv ]) => (
                    <option
                        key={key}>
                        {bpmv}
                    </option>
                ))}
            </Select>
        </Box>
        );
    }
}
  
  