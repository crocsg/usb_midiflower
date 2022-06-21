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
    render () {
        return (
        <Box color='white' bg='#293b25'>

            <h1>
            Scale
            </h1>
            <Label htmlFor='scale'>Scales</Label>
            <Select
                id='scale'
                name='Scale'
                defaultValue={this.state.scale}>
                    {Object.entries(this.state.scales).map(([ key, scale ]) => (
                    <option
                        key={key}>
                        {scale}
                    </option>
                ))}
            </Select>
            <p>
            {this.state.scales}
            </p>

            <h1>
            Root note      

            </h1>
            <h1>
            BPM
            </h1>
        </Box>
        );
    }
}
  
  