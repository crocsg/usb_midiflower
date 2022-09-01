import React from 'react';
import {Button, Flex, Box} from 'rebass';

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
            width:window.innerWidth,
            height:window.innerHeight
    
        };

        this.refresh =this.refresh.bind(this);
        
    }
    
    updateDimensions = () => {
        console.log ("resize", window.innerHeight, window.innerWidth);
        this.setState({ width: window.innerWidth, height: window.innerHeight });
        this.forceUpdate();
        window.location.reload(false);
        
      };
      
    componentDidMount ()
    {
        window.addEventListener('resize', this.updateDimensions);
        this.loaddata();
    }

    componentWillUnmount() {
        window.removeEventListener('resize', this.updateDimensions);
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
        
        
        return (
            <ResponsiveContainer width="99%" height={400}>
            <LineChart

            width={1024}
            height={400}
              data={this.state.data.mesures}
              margin={{
                top: 5,
                right: 5,
                left: 5,
                bottom: 5,
              }}
            >
              <CartesianGrid strokeDasharray="1 1" />
              <XAxis dataKey="id" />
              <YAxis />
              <Tooltip />
              <Legend />
              
              <Line type="monotone" dataKey="value" stroke="black" />
            </LineChart>
            </ResponsiveContainer>  
        )
    }
    render() {
        const items = this.state.data;
        const msg = this.state.fetchmsg;
        let datajsx = "";
        let graphjsx = "";

        
        if (items.mesures && items.mesures.length > 0)
        {
            console.log ("data analyze");
            datajsx = items.mesures.map((item) =><p> {item.id} / {item.value}</p>);
            graphjsx = this.renderchart ();
        }
        return (
            <Flex>
            <Box p={3} width={'5%'}></Box>
            <Box color='white' bg='#293b25b0' width={'90%'}>
                    <h1>Sensor Measures</h1>
                
                    
                    
                        {
                            graphjsx
                        }
                    
                    
               
                    <p><Button color="white" bg="#256C20" mr={2} onClick={this.refresh}>Refresh</Button></p>
                    <p>{msg}</p>
               </Box>
            <Box p={3} width={'5%'}></Box>
            </Flex>
          );
    }
  };
  
  