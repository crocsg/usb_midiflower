import React from 'react';
import {Button, Box} from 'rebass';

import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';
import '../App.css';
import isDev from './devdetect';

export default class Data extends React.Component {
    
    constructor(props) {
        super(props);
        this.state = {
            name:"Sensor measures",
            fetchmsg:"",
            data:[],

    
        };

        this.refresh =this.refresh.bind(this);
    }
    componentDidMount ()
    {
        this.loaddata();
    }

    refresh ()
    {
        console.log ("refresh");
        this.loaddata();
    }
    loaddata ()
    {
        let url = window.url_prefix + '/config/data/datadev.json';
        if (isDev()) {
            console.log('Development');
            
        } else {
            console.log('Production');
            url = window.url_prefix + '/config/data/data.json';
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
                        this.setState ({data:result})
                    }

                }
            )    
    }
    renderchart ()
    {
        console.log ("render chart");
        return (
            
            <LineChart
            width={1800}
            height={400}
              data={this.state.data.mesures}
              margin={{
                top: 5,
                right: 30,
                left: 30,
                bottom: 5,
              }}
            >
              <CartesianGrid strokeDasharray="1 1" />
              <XAxis dataKey="id" />
              <YAxis />
              <Tooltip />
              <Legend />
              
              <Line type="monotone" dataKey="value" stroke="white" />
            </LineChart>
          
        )
    }
    render() {
        const items = this.state.data;
        const msg = this.state.fetchmsg;
        let datajsx = "";
        let graphjsx = "";

        console.log ("before data analyze", items);
        if (items.mesures && items.mesures.length > 0)
        {
            console.log ("data analyze");
            datajsx = items.mesures.map((item) =><p> {item.id} / {item.value}</p>);
            graphjsx = this.renderchart ();
        }
        return (
            <Box color='white' bg='#293b25'>
                    <p>{this.state.name}</p>
                
                    <div className='graphbox'>
                        {
                            graphjsx
                        }
                    </div>
               
                    <p><Button color="white" bg="#256C20" mr={2} onClick={this.refresh}>Refresh</Button></p>
                    <p>{msg}</p>
               </Box>
            
          );
    }
  };
  
  